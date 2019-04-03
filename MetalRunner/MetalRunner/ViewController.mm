//
//  ViewController.m
//  RendererDemo
//
//  Created by kusugawa on 2019/3/22.
//  Copyright © 2019年 kusugawa. All rights reserved.
//

#import "ViewController.h"


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
    _application->resize((uint32_t)_view.frame.size.width,(uint32_t)_view.frame.size.height);
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
    _gameViewSize = { (uint32_t)_view.frame.size.width,(uint32_t)_view.frame.size.height };
    //NSBundle * bundle = [NSBundle mainBundle];
    NSString* documentPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) firstObject];
    Ks::IArch * arch = Ks::CreateStdArchieve( std::string( [documentPath cStringUsingEncoding:NSUTF8StringEncoding] ));
    _application = GetApplication();
    _application->initialize( (__bridge void*) _metalLayer , arch);
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
    _application->tick();
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
