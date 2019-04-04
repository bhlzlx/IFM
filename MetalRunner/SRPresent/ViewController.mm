//
//  ViewController.m
//  SRPresent
//
//  Created by pixelsoft on 2019/4/4.
//  Copyright © 2019 kusugawa. All rights reserved.
//

#import "ViewController.h"
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

@interface ViewController ()
@end

@implementation ViewController


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
    _displayLink = [CADisplayLink displayLinkWithTarget: self selector: @selector(renderTick)];
    [_displayLink addToRunLoop: [NSRunLoop currentRunLoop] forMode: NSDefaultRunLoopMode];
}

-(void)viewWillLayoutSubviews {
    
    [_metalLayer setFrame: _metalLayer.superlayer.frame];
    [_metalLayer setDrawableSize: _metalLayer.superlayer.frame.size];
    
    _application->resize((uint32_t)_view.frame.size.width,(uint32_t)_view.frame.size.height);
    _gameViewSize = { (uint32_t)_view.frame.size.width,(uint32_t)_view.frame.size.height };
}


-(void) renderTick {
    bool rst = false;
    _application->tick();
}

-(void)dealloc {
    [_displayLink invalidate];
}

@end
