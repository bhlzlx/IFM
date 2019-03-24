#include "TextureMTL.h"
#include "ContextMTL.h"
#include "MTLTypeMapping.mm"

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
    
    MTLLoadAction MappingLoadActionMTL(RTLoadAction loadAction)
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
    
    uint32_t PixelBytes( KsFormat _format ){
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

    TextureMTL* TextureMTL::createTexture( const TextureDescription& _desc, id<MTLTexture> _texture ) {
        GetContext(context);
        MTLPixelFormat format = KsFormatToMTL( _desc.format );
        if( format == MTLPixelFormatInvalid )
            return nullptr;
        MTLTextureDescriptor* descriptor = nil;
        BOOL mipmapped =_desc.mipmapLevel == 1 ? NO : YES;
        
        if( _desc.type == TextureType::Texture2D ){
            descriptor = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:format width:_desc.width height:_desc.height mipmapped: mipmapped];
        }else if( _desc.type == TextureType::TextureCube ) {
            descriptor = [MTLTextureDescriptor textureCubeDescriptorWithPixelFormat:format size:_desc.width mipmapped:mipmapped];
        }else if( _desc.type == TextureType::Texture2DArray || _desc.type == TextureType::Texture3D ) {
            descriptor = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:format width:_desc.width height:_desc.height mipmapped: mipmapped];
            descriptor.arrayLength = _desc.depth;
        }else if( _desc.type == TextureType::TextureCubeArray ){
            descriptor = [MTLTextureDescriptor textureCubeDescriptorWithPixelFormat:format size:_desc.width mipmapped:mipmapped];
            descriptor.arrayLength = _desc.depth;
        }else {
            assert(false);
            return nullptr;
        }
        descriptor.mipmapLevelCount = _desc.mipmapLevel;
        descriptor.resourceOptions = MTLResourceStorageModePrivate;
        descriptor.usage = MTLTextureUsageRenderTarget;
        id<MTLTexture> texture = [context->getDevice() newTextureWithDescriptor:descriptor];
        
        TextureMTL* newTexture = new TextureMTL();
        newTexture->m_texture = texture;
        newTexture->m_description = _desc;
        return newTexture;
    }
    
    void setSubData(const void *_data, size_t _length, const Ks::ImageRegion &_region) {
        // queue update
    }
    
    void release() {
        
    }
    
}
