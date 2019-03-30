//
//  KsView.h
//  MetalRunner
//
//  Created by kusugawa on 2019/3/30.
//  Copyright © 2019年 kusugawa. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <QuartzCore/CAMetalLayer.h>

NS_ASSUME_NONNULL_BEGIN

@interface KsView : NSView
{
    CAMetalLayer* _metalLayer;
}

-(CAMetalLayer*) metalLayer;
-(void) setupMetalLayer;

@end

NS_ASSUME_NONNULL_END
