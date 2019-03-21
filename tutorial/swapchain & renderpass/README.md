# 前言
因为iOS模拟器并不支持metal，编写metal程序还是有不太方便，所以搭建一个OSX上运行的metal环境会更加方便一些，这里我们不使用绝大部分文章使用的MTKView,原因是它封装了不少细节，而我们是学习，就是要探究这个metal是怎么运作的，所以我们从零开始写一个Mac OS X Metal App。

# 准备工作
第一篇出于简单起见，让大家初识环境，虽然简单，但是还是可能会有一些不少人不完全明白东西，请大家保持平常心一步一步看。

Metal 资源类型：
```
id<MTLDevice>
id<MTLCommandQueue>
id<MTLTexture>
id<MTLCommandBuffer>
MTLRenderPassDescriptor
```
### 驱动渲染逻辑的工具类
```
CVDisplayLinkRef
```

### 信号量（维持交换链动作）
```
dispatch_semaphore_t
```

# 开始编写：
创建工程的时候务必创建 Macos 普通的App工程，当然选Objective-C Metal 游戏工程的基础上改也没问题的。

给ViewController添加如下几个成员
```
NSView* _view;
CAMetalLayer* _metalLayer;
id<MTLDevice> _metalDevice;
id<MTLTexture> _depthStencil;
CVDisplayLinkRef _displayLink;    
id<MTLCommandQueue> _queue;
dispatch_semaphore_t _semaphore;
```

添加全局方法
```
static CVReturn renderCallback(CVDisplayLinkRef displayLink,
const CVTimeStamp *inNow,
const CVTimeStamp *inOutputTime,
CVOptionFlags flagsIn,
CVOptionFlags *flagsOut,
void *displayLinkContext)
{
GameViewController* controller = (__bridge GameViewController*)displayLinkContext;
return [controller renderTick: inOutputTime];
}
```
给ViewController添加方法
```
-(void) startRenderTick {
CVDisplayLinkStart(_displayLink);
}
-(void) stopRenderTick {
CVDisplayLinkStop(_displayLink);
}
-(CVReturn) renderTick:(const CVTimeStamp*)time { 
}
```

ViewController 的 viewDidLoad 方法实现
```
- (void)viewDidLoad
{
[super viewDidLoad];
_view = self.view;
_metalDevice = MTLCreateSystemDefaultDevice();
if(!_metalDevice){
return;
}
_queue = [_metalDevice newCommandQueue];
// create CAMetalLayer
_metalLayer = [[CAMetalLayer alloc] init];
_metalLayer.device = _metalDevice;
_metalLayer.framebufferOnly = YES;
[_metalLayer setFrame:_view.frame];
[_view.layer addSublayer:_metalLayer];
// create display link
CGDirectDisplayID   displayID = CGMainDisplayID();
CVReturn error = kCVReturnSuccess;
error = CVDisplayLinkCreateWithCGDisplay(displayID, &_displayLink);
if (error) {
NSLog(@"DisplayLink created with error:%d", error);
_displayLink = NULL;
}
CVDisplayLinkSetOutputCallback(_displayLink, renderCallback, (__bridge void *)self);
// create semaphore
_semaphore = dispatch_semaphore_create(3);
[self startRenderTick];
}
```

我们借助OSX平台的CVDispalyLink相关的工具驱动起来了渲染方法，程序会不停的调用
renderTick:(const CVTimeStamp*)time

接下来我们会在这个方法里写渲染逻辑
为了方便大家复制粘贴我把整个渲染代码粘贴在这，稍候讲解每一个细节
```
-(CVReturn) renderTick:(const CVTimeStamp*)time {
dispatch_semaphore_wait( _semaphore, DISPATCH_TIME_FOREVER);
id<CAMetalDrawable> drawable = _metalLayer.nextDrawable;
id<MTLTexture> colorTexture = drawable.texture;
//
if( !drawable || !colorTexture ){
return kCVReturnError;
}
id<MTLCommandBuffer> cmdBuffer = [_queue commandBuffer];
// create depth stencil texture
if(!_depthStencil) {
MTLTextureDescriptor* desc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatDepth32Float width:colorTexture.width height:colorTexture.height mipmapped:NO];
desc.resourceOptions = MTLResourceStorageModePrivate;
desc.usage = MTLTextureUsageRenderTarget;
_depthStencil = [_metalDevice newTextureWithDescriptor:desc];
} else {
if( _depthStencil.width != colorTexture.width || _depthStencil.height != colorTexture.height ){
MTLTextureDescriptor* desc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatDepth32Float width:colorTexture.width height:colorTexture.height mipmapped:NO];
desc.resourceOptions = MTLResourceStorageModePrivate;
desc.usage = MTLTextureUsageRenderTarget;
_depthStencil = [_metalDevice newTextureWithDescriptor:desc];
}
}
// create main render pass
MTLRenderPassDescriptor* renderPassDescriptor = [MTLRenderPassDescriptor renderPassDescriptor];
renderPassDescriptor.colorAttachments[0].texture = colorTexture;
renderPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
renderPassDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
renderPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake( 1.0, 0.0, 1.0, 1.0 );
renderPassDescriptor.depthAttachment.texture = _depthStencil;
renderPassDescriptor.depthAttachment.loadAction = MTLLoadActionClear;
renderPassDescriptor.depthAttachment.storeAction = MTLStoreActionStore;
renderPassDescriptor.depthAttachment.clearDepth = 1.0f;

id<MTLRenderCommandEncoder> encoder = [cmdBuffer renderCommandEncoderWithDescriptor: renderPassDescriptor];
[encoder endEncoding];
//[cmdBuffer presentDrawable: drawable]
[cmdBuffer addCompletedHandler:^(id<MTLCommandBuffer> _commandBuffer) {
dispatch_semaphore_signal(self->_semaphore);
[drawable present];
}];
[cmdBuffer commit];
return kCVReturnSuccess;
}
```

# 详解
## 驱动渲染调用
CVDisplayLink* 相关的东西对于metal的机制来说并没有关系，它只是个驱动渲染的方法，关于它我们不去细究，它并不重要，若想详细了解可以查阅苹果开发者文档。

## 交换链、队列、基本模型
首先，dispatch_semaphore_t 是什么东西？在metal里没有交换链这个对象，但实际依然有交换链的概念，相比于vulkan来讲，苹果的设计更加巧妙，我们仅用少量代码就实现了交换链的机制。metal的流水线模型，我们是向指令队列里不断地添加指令，GPU收到指令然后再去处理，其实相当于一个生产者消费者模型，那么这个模型就有个问题，如果消费者不能及时消费完，那么生产者不停的提供产品就会产生问题，我们需要一个机制，让队列里不能有过多的渲染指令，一般来讲队列里最多有三帧的指令比较合适，只要保证队列里的指令永不为空，我们就可以极大限度的压榨GPU计算能力，岂不美哉！
所以这里有一个策略，只要这个semaphore的值不等于零，我们就给它减一，然后向队列里投递一个 command buffer, 这个command buffer持有一帧的渲染指令，当这个 command buffer被GPU消费完之后我们再给semaphore自增一，如果semaphorer的值等于0，那我们就等待，这样队列一般能保持不空，且队列不会持有超过三帧的未渲染完成的指令，CPU不会盲目的生产指令，GPU也可以保持忙碌状态，苹果的这个设计是相当巧妙的！vulkan 的swapchain则是另外一个思路，这是外话。

## 指令缓冲（command buffer）
指令缓冲是用来记录渲染操作的一个对象，相当于一张纸记录着一个人一天要做的事情，这个东西就是要告诉GPU应该使用什么资源做什么操作，不过这个执行并不是让GPU立即执行的，而是扔到队列里等待GPU执行它。

## Render Pass
render pass 也有其它的程序员称之为 RenderTarget(RT), Framebuffer Object( FBO ），具体来讲它是一个画布，或者可以认为是流水线的最终输出目标（出口），出口可能有一个，两个，三个或者更多。既然我们明白它的作用了，那么也能猜到它有哪些行为。
### 1. 绑定纹理
指定这个render pass实际输出到什么纹理上
### 2.加载时行为
也许我们在画画之前需要一张空白纸，也许我们要接上次的结果继续画，也许我十分确信这次画的东西会完全把之前的结果覆盖了 ( 深究会牵扯到纹理布局问题)
### 3.实际存储行为
这个可能比较难理解，为什么还要指定存储行为？我渲染了肯定要存的啊，哈哈，实际并不一定要存的，有时候tiled rendering时只是过个场，不需要存可以省好多带宽呢，这是个比较高级的主题，现在先不谈它。
### 4.设置清屏值
清屏清成什么颜色呢？

## CAMetalLayer
CAMetalLayer为 render pass 提供输出目标，一个CAMetalLayer上可以不停地取Texture对象，渲染完，呈现出来之后可以给下一轮重用，所以CAMetalLayer实际上持有很多纹理对象。

## RenderCommandEncoder 
可以简单理解为向comand buffer写指令的一个东西，但也不完全有这么简单，仔细看一个你会发现它是依赖于render pass的，这里仅给大家提个醒，依赖于render pass也是有原因的，这里不作深究。

看到这里大家再回头看下渲染方法里的逻辑，是不是感觉逻辑更加清晰了！
等待可以走渲染逻辑的条件(semaphore-1)->获取纹理->准备指令->生成render pass->将render pass相关操作写入指令（这里仅清屏）->提交指令，进入队列（执行完成后semaphore+1, 呈现渲染结果）

以上就是一个简单的清屏操作牵扯出来的东西，一点V小的工作，希望能帮到大家。
