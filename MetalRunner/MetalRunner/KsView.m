//
//  KsView.m
//  MetalRunner
//
//  Created by kusugawa on 2019/3/30.
//  Copyright © 2019年 kusugawa. All rights reserved.
//

#import "KsView.h"

@implementation KsView

-(void) setupMetalLayer {
    if( nil == _metalLayer ) {
        _metalLayer = [CAMetalLayer layer];
        [self.layer addSublayer:_metalLayer];
    }    
}

- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];
    // Drawing code here.
}

-(CAMetalLayer*) metalLayer {
    return _metalLayer;
}

- (void)setFrameSize:(NSSize)newSize {
    [super setFrameSize:newSize];
    [_metalLayer setFrame: _metalLayer.superlayer.frame];
    [_metalLayer setDrawableSize: newSize];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"resize" object:self userInfo:nil];
}

@end
