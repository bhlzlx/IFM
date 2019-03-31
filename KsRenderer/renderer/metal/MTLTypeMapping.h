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
    
    inline uint32_t PixelBits( KsFormat _format ){
        switch (_format) {
            case KsInvalidFormat:
                return 0;
            case KsRGBA8888_UNORM:
            case KsBGRA8888_UNORM:
            case KsRGBA8888_SNORM:
                return 32;
            case KsRGB565_PACKED:
            case KsRGBA5551_PACKED:
                return 16;
            case KsRGBA_F16:
                return 64;
            case KsRGBA_F32:
                return 128;
            case KsDepth24FStencil8:
            case KsDepth32F:
                return 32;
            case KsDepth32FStencil8:
                return 40;
            case KsETC2_LINEAR_RGBA:
            case KsEAC_RG11_UNORM:
                return 8;
            case KsBC3_LINEAR_RGBA:
                return 8;
            case KsPVRTC_LINEAR_RGBA:
                return 4;
            default:
                return 0;
        }
        return 0;
    }
    
    inline MTLLoadAction LoadActionToMTL( RTLoadAction _action ) {
        switch( _action ) {
            case Keep: return MTLLoadActionLoad;
            case Clear: return MTLLoadActionClear;
            case DontCare: return MTLLoadActionDontCare;
        }
        return MTLLoadActionLoad;
    }
    
    inline bool IsStencilFormat(Ks::KsFormat _format) {
        switch (_format) {
            case KsDepth32FStencil8:
            case KsDepth24FStencil8:
                return true;
            default:
                break;
        }
        return false;
    }
    
    MTLVertexFormat VertexFormatToMTL( VertexType _vtType ) {
        switch( _vtType ){
            case VertexTypeFloat1:
                return MTLVertexFormatFloat;
            case VertexTypeFloat2:
                return MTLVertexFormatFloat2;
            case VertexTypeFloat3:
                return MTLVertexFormatFloat3;
            case VertexTypeFloat4:
                return MTLVertexFormatFloat4;
            case VertexTypeHalf2:
                return MTLVertexFormatHalf2;
            case VertexTypeHalf4:
                return MTLVertexFormatHalf4;
            case VertexTypeUByte4:
                return MTLVertexFormatUChar4;
            case VertexTypeUByte4N:
                return MTLVertexFormatUChar4Normalized;
        }
        return MTLVertexFormatInvalid;
    }
    
    static inline MTLSamplerMipFilter MipmapFilerToMTL( TextureFilter _filter)
    {
        switch (_filter)
        {
            case TexFilterNone:
            case TexFilterPoint:
                return MTLSamplerMipFilterNearest;
            case TexFilterLinear:
                return MTLSamplerMipFilterLinear;
        }
        return MTLSamplerMipFilterNotMipmapped;
    }
    
    static inline MTLCullMode CullModeToMTL( CullMode _mode)
    {
        switch (_mode)
        {
            case None:
                return MTLCullModeNone;
            case Back:
                return MTLCullModeBack;
            case Front:
                return MTLCullModeFront;
            case FrontAndBack:
                return MTLCullModeFront;
        }
        return MTLCullModeNone;
    }
    
    static inline MTLPrimitiveType TopologyToMTL( TopologyMode _mode )
    {
        switch (_mode)
        {
            case TMPoints: return MTLPrimitiveTypePoint;
            case TMLineStrip: return MTLPrimitiveTypeLineStrip;
            case TMLineList: return MTLPrimitiveTypeLine;
            case TMTriangleStrip: return MTLPrimitiveTypeTriangleStrip;
            case TMTriangleList: return MTLPrimitiveTypeTriangle;
            default:
                break;
        }
        return MTLPrimitiveTypeTriangle;
    }
    
    static inline MTLPrimitiveTopologyClass TopolygyPolygonModeMTL( TopologyMode _mode )
    {
        switch (_mode)
        {
            case TMPoints:
                return MTLPrimitiveTopologyClassPoint;
            case TMLineStrip:
            case TMLineList:
                return MTLPrimitiveTopologyClassLine;
            case TMTriangleStrip:
            case TMTriangleList:
            case TMTriangleFan:
                return MTLPrimitiveTopologyClassTriangle;
            case TMCount:
                break;
        }
        return MTLPrimitiveTopologyClassTriangle;
    }
    
    static inline MTLWinding FrontFaceToMTL( WindingMode _mode)
    {
        switch (_mode)
        {
            case Clockwise: return MTLWindingClockwise;
            case CounterClockwise: return MTLWindingCounterClockwise;
        }
        return MTLWindingCounterClockwise;
    }
    
    static inline MTLCompareFunction CompareFunctionToMTL( CompareFunction _op)
    {
        switch (_op)
        {
            case Never: return MTLCompareFunctionNever;
            case Less: return MTLCompareFunctionLess;
            case Equal: return MTLCompareFunctionEqual;
            case LessEqual: return MTLCompareFunctionLessEqual;
            case Greater: return MTLCompareFunctionGreater;
            case GreaterEqual: return MTLCompareFunctionGreaterEqual;
            case Always: return MTLCompareFunctionAlways;
        }
        //VK_COMPARE_OP_NOT_EQUAL
        return MTLCompareFunctionAlways;
    }
    
    static inline MTLStencilOperation StencilOpToMTL( StencilOperation _op)
    {
        switch (_op)
        {
            case StencilOpKeep:return MTLStencilOperationKeep;
            case StencilOpZero: return MTLStencilOperationZero;
            case StencilOpReplace: return MTLStencilOperationReplace;
            case StencilOpIncrSat:return MTLStencilOperationIncrementClamp;
            case StencilOpDecrSat:return MTLStencilOperationDecrementClamp;
            case StencilOpInvert:return MTLStencilOperationInvert;
            case StencilOpInc:return MTLStencilOperationIncrementWrap;
            case StencilOpDec:return MTLStencilOperationDecrementWrap;
        }
        return MTLStencilOperationKeep;
    }
    
    static inline MTLBlendFactor BlendFactorToMTL( BlendFactor _factor)
    {
        switch (_factor)
        {
            case Zero: return MTLBlendFactorZero;
            case One: return MTLBlendFactorOne;
            case SourceColor: return MTLBlendFactorSourceColor;
            case InvertSourceColor: return MTLBlendFactorOneMinusSourceColor;
            case SourceAlpha: return MTLBlendFactorSourceAlpha;
            case InvertSourceAlpha: return MTLBlendFactorOneMinusSourceAlpha;
            case DestinationColor: return MTLBlendFactorDestinationColor;
            case InvertDestinationColor: return MTLBlendFactorOneMinusDestinationColor;
            case DestinationAlpha: return MTLBlendFactorDestinationAlpha;
            case InvertDestinationAlpha: return MTLBlendFactorOneMinusDestinationAlpha;
            case SourceAlphaSat: return MTLBlendFactorSourceAlphaSaturated;
        }
        return MTLBlendFactorOne;
    }
    
    static inline MTLBlendOperation BlendOpToMTL( BlendOperation _op)
    {
        switch (_op)
        {
            case BlendOpAdd: return MTLBlendOperationAdd;
            case BlendOpSubtract: return MTLBlendOperationSubstract;
            case BlendOpRevsubtract: return MTLBlendOperationReverseSubtract;
        }
        return MTLBlendOperationAdd;
    }
}
