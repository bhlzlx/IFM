//
//  ViewController.h
//  MTLRenderer
//
//  Created by kusugawa on 2019/3/21.
//  Copyright © 2019年 kusugawa. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <QuartzCore/CAMetalLayer.h>
#import <Metal/Metal.h>

@interface ViewController : NSViewController
{
    NSView* _view;
    CAMetalLayer* _metalLayer;
    id<MTLDevice> _metalDevice;
    id<MTLTexture> _depthStencil;
    CVDisplayLinkRef _displayLink;
    id<MTLCommandQueue> _queue;
    dispatch_semaphore_t _semaphore;
}

-(void) renderTick;

@end

