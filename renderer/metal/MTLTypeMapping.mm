//
//  Header.h
//  RendererMTL
//
//  Created by kusugawa on 2019/3/23.
//  Copyright © 2019年 kusugawa. All rights reserved.
//
#pragma once
#include <KsRenderer.h>
#include <Metal/Metal.h>

namespace Ks {
    
    inline MTLPixelFormat KsFormatToMTL( KsFormat _format ) {
        switch (_format) {
            case KsRGBA8888_UNORM: return MTLPixelFormatRGBA8Unorm;
            case KsRGBA8888_SNORM: return MTLPixelFormatRGBA8Snorm;
            case KsBGRA8888_UNORM: return MTLPixelFormatBGRA8Unorm;
#if TARGET_OS_IPHONE
            case KsRGB565_PACKED: return MTLPixelFormatB5G6R5Unorm;
            case KsRGBA5551_PACKED: return MTLPixelFormatA1BGR5Unorm;
#endif
            case KsRGBA_F16: return MTLPixelFormatRGBA16Float;
            case KsRGBA_F32: return MTLPixelFormatRGBA32Float;
            case KsDepth24FStencil8: return MTLPixelFormatDepth24Unorm_Stencil8;
            case KsDepth32F: return MTLPixelFormatDepth32Float;
            case KsDepth32FStencil8: return MTLPixelFormatDepth32Float_Stencil8;
                
#if TARGET_OS_IPHONE
            case KsETC2_LINEAR_RGBA: return MTLPixelFormatEAC_RGBA8;
            case KsPVRTC_LINEAR_RGBA: return MTLPixelFormatPVRTC_RGBA_4BPP;
#else
            case KsBC3_LINEAR_RGBA: return MTLPixelFormatBC3_RGBA;
#endif
            default:
                break;
        }
        return MTLPixelFormatInvalid;
    }
    
    inline KsFormat MTLFormatToKs( MTLPixelFormat _format ) {
        switch( _format ) {
            case MTLPixelFormatRGBA8Unorm: return KsRGBA8888_UNORM;
            case MTLPixelFormatRGBA8Snorm: return KsRGBA8888_SNORM;
            case MTLPixelFormatBGRA8Unorm: return KsBGRA8888_UNORM;
#if TARGET_OS_IPHONE
            case MTLPixelFormatB5G6R5Unorm: return KsRGB565_PACKED;
            case MTLPixelFormatA1BGR5Unorm: return KsRGBA5551_PACKED;
#endif
            case MTLPixelFormatRGBA16Float: return KsRGBA_F16;
            case MTLPixelFormatRGBA32Float: return KsRGBA_F32;
            case MTLPixelFormatDepth24Unorm_Stencil8: return KsDepth24FStencil8;
            case MTLPixelFormatDepth32Float: return KsDepth32F;
            case MTLPixelFormatDepth32Float_Stencil8: return KsDepth32FStencil8;
                
#if TARGET_OS_IPHONE
            case MTLPixelFormatEAC_RGBA8: return KsETC2_LINEAR_RGBA;
            case MTLPixelFormatPVRTC_RGBA_4BPP: return KsPVRTC_LINEAR_RGBA;
#else
            case MTLPixelFormatBC3_RGBA: return KsBC3_LINEAR_RGBA;
#endif
            default:
                break;
        }
        return KsInvalidFormat;
    }
    
    inline MTLLoadAction MappingLoadActionMTL(RTLoadAction loadAction)
    {
        switch (loadAction) {
            case Keep:
                return MTLLoadActionLoad;
            case Clear:
                return MTLLoadActionClear;
            case DontCare:
                return MTLLoadActionDontCare;
            default:
                break;
        }
    }
    
    inline uint32_t PixelBytes( KsFormat _format ){
        switch (_format) {
            case KsInvalidFormat:
                return 0;
            case KsRGBA8888_UNORM:
                return 4;
            case KsBGRA8888_UNORM:
                return 4;
            case KsRGBA8888_SNORM:
                return 8;
            case KsRGB565_PACKED:
                return 2;
            case KsRGBA5551_PACKED:
                return 2;
            case KsRGBA_F16:
                return 8;
            case KsRGBA_F32:
                return 16;
            case KsDepth24FStencil8:
                return 4;
            case KsDepth32F:
                return 4;
            case KsDepth32FStencil8:return 5;
                break;
            case KsETC2_LINEAR_RGBA:
                return 0;
            case KsBC3_LINEAR_RGBA:
                return 0;
            case KsPVRTC_LINEAR_RGBA:
                return 0;
        }
        return 0;
    }
    
}
