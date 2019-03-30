//
//  ViewController.m
//  RendererDemo
//
//  Created by kusugawa on 2019/3/22.
//  Copyright © 2019年 kusugawa. All rights reserved.
//

#import "ViewController.h"
#import "ks/io/io.h"
#import <KsRenderer/KsRenderer.h>
#include <QuartzCore/CAMetalLayer.h>
#import "KsView.h"

@interface ViewController : NSViewController
{
    KsView* _view;
    __weak CAMetalLayer* _metalLayer;
    CVDisplayLinkRef _displayLink;
    Ks::IContext* _context;
    Ks::IView* _gameView;
    Ks::Size<uint32_t> _gameViewSize;
}
@end

@implementation ViewController

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

-(void)resize {
    _gameView->resize( _metalLayer.frame.size.width, _metalLayer.frame.size.height);
    _gameViewSize = { (uint32_t)_view.frame.size.width,(uint32_t)_view.frame.size.height };
}

- (void)viewDidLoad {
    [super viewDidLoad];
    //
    _view = (KsView*)self.view;
    [_view setupMetalLayer];
    _metalLayer = [_view metalLayer];
    NSNotificationCenter * center = [NSNotificationCenter defaultCenter];
    [center addObserver:self selector:@selector(resize) name:@"resize" object:nil];

    _metalLayer.framebufferOnly = YES;
    //[_metalLayer setFrame:_view.frame];
    //[_view.layer addSublayer:_metalLayer];
    _gameViewSize = { (uint32_t)_view.frame.size.width,(uint32_t)_view.frame.size.height };
    _context = Ks::GetContextMetal();
    //NSBundle * bundle = [NSBundle mainBundle];
    NSString* documentPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) firstObject];
    Ks::IArch * arch = Ks::CreateStdArchieve( std::string( [documentPath cStringUsingEncoding:NSUTF8StringEncoding] ));
    _context->initialize(arch, ( __bridge void * )_metalLayer);
    _gameView = _context->createView(( __bridge void * )_metalLayer);
    auto mainRenderPass = _gameView->getRenderPass();
    Ks::RpClear clear = {} ;{
        clear.colors[0] = { 1.0f, 0.5f, 0.5f, 1.0f};
        clear.depth = 1.0f;
        clear.stencil = 0.0f;
    }
    mainRenderPass->setClear(clear);
    // Do any additional setup after loading the view.
    // create display link
    CGDirectDisplayID   displayID = CGMainDisplayID();
    CVReturn error = kCVReturnSuccess;
    error = CVDisplayLinkCreateWithCGDisplay(displayID, &_displayLink);
    if (error) {
        NSLog(@"DisplayLink created with error:%d", error);
        _displayLink = NULL;
    }
    CVDisplayLinkSetOutputCallback(_displayLink, renderCallback, (__bridge void *)self);
    [self startRenderTick];
}


-(void) renderTick {
    bool rst = false;
    rst = _gameView->beginFrame();
    if( rst ){
        auto mainRenderPass = _gameView->getRenderPass();
        rst = mainRenderPass->begin(_gameViewSize.width, _gameViewSize.height);
        mainRenderPass->end();
    }
    _gameView->endFrame();
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
