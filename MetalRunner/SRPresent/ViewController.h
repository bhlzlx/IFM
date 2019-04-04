//
//  ViewController.h
//  SRPresent
//
//  Created by pixelsoft on 2019/4/4.
//  Copyright © 2019 kusugawa. All rights reserved.
//

#import <UIKit/UIKit.h>
#include <QuartzCore/CAMetalLayer.h>
#import <QuartzCore/CADisplayLink.h>
#import <KsRendereriOS/KsRenderer.h>
#import "KsView.h"
#import "ks/io/io.h"
#import "Triangle.h"

@interface ViewController : UIViewController
{
    __weak CAMetalLayer* _metalLayer;
    KsView* _view;
    CADisplayLink* _displayLink;
    Ks::Size<uint32_t> _gameViewSize;
    KsApplication* _application;
    //
    Ks::IArch* _archieve;
    Ks::Viewport _viewport;
    Ks::Scissor _scissor;
}


@end

