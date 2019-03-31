#pragma once

#include <KsRenderer.h>
#include <Metal/Metal.h>
#include <QuartzCore/CAMetalLayer.h>
#include <Cocoa/Cocoa.h>

enum BufferUsageFlagBits :uint8_t {
    BufferUsageStaticVBO = 0,
    BufferUsageDynamicVBO,
    BufferUsageUniform,
    BufferUsageIBO,
    BufferUsageSSBO,
};

typedef uint32_t BufferUsageFlags;
