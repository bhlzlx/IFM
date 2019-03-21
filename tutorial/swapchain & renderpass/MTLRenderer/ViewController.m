//
//  ViewController.m
//  MTLRenderer
//
//  Created by kusugawa on 2019/3/21.
//  Copyright © 2019年 kusugawa. All rights reserved.
//

#import "ViewController.h"

static CVReturn renderCallback(CVDisplayLinkRef displayLink,
                               const CVTimeStamp *inNow,
                               const CVTimeStamp *inOutputTime,
                               CVOptionFlags flagsIn,
                               CVOptionFlags *flagsOut,
                               void *displayLinkContext)
{
    ViewController* controller = (__bridge ViewController*)displayLinkContext;
    [controller performSelectorOnMainThread:@selector(renderTick) withObject:nil waitUntilDone:YES];
    return kCVReturnSuccess;
}

@implementation ViewController

- (void)viewDidLoad {
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

    // Do any additional setup after loading the view.
}


-(void) renderTick {
    dispatch_semaphore_wait( _semaphore, DISPATCH_TIME_FOREVER);
    id<CAMetalDrawable> drawable = _metalLayer.nextDrawable;
    id<MTLTexture> colorTexture = drawable.texture;
    //
    if( !drawable || !colorTexture ){
        return;
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
}

- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];
    // Update the view, if already loaded.
}

-(void) startRenderTick {
    CVDisplayLinkStart(_displayLink);
}
-(void) stopRenderTick {
    CVDisplayLinkStop(_displayLink);
}

@end
