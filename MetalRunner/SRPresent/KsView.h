//
//  KsView.h
//  SRPresent
//
//  Created by pixelsoft on 2019/4/4.
//  Copyright © 2019 kusugawa. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

NS_ASSUME_NONNULL_BEGIN

@interface KsView : UIView
{
    CAMetalLayer* _metalLayer;
}

-(void) setupMetalLayer;
-(CAMetalLayer*) metalLayer;

@end

NS_ASSUME_NONNULL_END
