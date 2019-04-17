//
//  ViewController.h
//  RendererDemo
//
//  Created by kusugawa on 2019/3/22.
//  Copyright © 2019年 kusugawa. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include <QuartzCore/CAMetalLayer.h>
#import "ks/io/io.h"
#import <KsRenderer/KsRenderer.h>
#import "KsView.h"
#import "Triangle.h"

@interface ViewController : NSViewController
{
    __weak CAMetalLayer* _metalLayer;
    KsView* _view;
    CVDisplayLinkRef _displayLink;
    Ks::Size<uint32_t> _gameViewSize;
    KsApplication* _application;
    //
    Ks::IArch* _archieve;
    Ks::Viewport _viewport;
    Ks::Scissor _scissor;
}

@end
