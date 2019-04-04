#pragma once

#include <KsRenderer.h>
#include <Metal/Metal.h>
#include <QuartzCore/CAMetalLayer.h>
#if TARGET_OS_IPHONE
#include <UIKit/UIKit.h>
#else
#include <Cocoa/Cocoa.h>
#endif


enum BufferUsageFlagBits :uint8_t {
    BufferUsageStaticVBO = 0,
    BufferUsageDynamicVBO,
    BufferUsageUniform,
    BufferUsageIBO,
    BufferUsageSSBO,
};

typedef uint32_t BufferUsageFlags;
