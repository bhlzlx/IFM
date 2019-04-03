//
//  AppDelegate.m
//  MTLRenderer
//
//  Created by kusugawa on 2019/3/21.
//  Copyright © 2019年 kusugawa. All rights reserved.
//

#import "AppDelegate.h"
#import <KsRenderer/KsApplication.h>

@interface AppDelegate ()

@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application
    auto window = [[NSApplication sharedApplication] mainWindow];
    auto app = GetApplication();
    [window setTitle:[NSString stringWithUTF8String: app->title()]];
}


- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}


@end
