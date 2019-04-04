//
//  KsView.m
//  SRPresent
//
//  Created by pixelsoft on 2019/4/4.
//  Copyright © 2019 kusugawa. All rights reserved.
//

#import "KsView.h"

@implementation KsView

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
}
*/

-(void) setupMetalLayer {
    _metalLayer = [[CAMetalLayer alloc] init];
    [self.layer addSublayer:_metalLayer];
}

-(CAMetalLayer*) metalLayer {
    return _metalLayer;
}

@end
