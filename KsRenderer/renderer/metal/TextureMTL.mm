#include "TextureMTL.h"
#include "ContextMTL.h"
#include "QueueMTL.h"
#include "MTLTypeMapping.h"

namespace Ks {
    
    ITexture* ContextMTL::createTexture(const TextureDescription& _desc, TextureUsageFlags _usage ) {
        return TextureMTL::createTexture( _desc, _usage, nil );
    }
    
    TextureMTL* TextureMTL::createTexture( const TextureDescription& _desc,TextureUsageFlags _usage, id<MTLTexture> _texture ) {
        GetContext(context);
        MTLTextureUsage usage = MTLTextureUsageUnknown;
        //MTLResourceOptions options = MTLCPUCacheModeDefaultCache;
        if( _usage != 0 ) {
            if( _usage & TextureUsageSampled ) {
                usage |= ( MTLTextureUsageShaderRead | MTLTextureUsageShaderWrite );
            }
            if( _usage & TextureUsageColorAttachment ) {
                usage |= MTLTextureUsageRenderTarget;
            }
            if( _usage & TextureUsageDepthStencilAttachment ) {
                usage |= MTLTextureUsageRenderTarget;
            }
            if( _usage & TextureUsageStorage ) {
                usage |= MTLTextureUsagePixelFormatView;
            }
        }
        else
        {
            usage = MTLTextureUsageShaderRead | MTLTextureUsageShaderWrite | MTLTextureUsageRenderTarget;
        }
        
        MTLPixelFormat format = KsFormatToMTL( _desc.format );
        if( format == MTLPixelFormatInvalid )
            return nullptr;
        if( _texture == nil ) {
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
            descriptor.usage = usage;
            _texture = [context->getDevice() newTextureWithDescriptor:descriptor];
        }
        TextureMTL* newTexture = new TextureMTL();
        newTexture->m_texture = _texture;
        newTexture->m_description = _desc;
        return newTexture;
    }
    
    void TextureMTL::setSubData( const void * _data, size_t _length, const TextureRegion& _region ) {
        // update var queue
        GetContext(context)
        context->getGraphicsQueue().updateTexture( this, _data, _length, _region);
    }
    //
    void TextureMTL::setSubData(const void * _data, size_t _length, const TextureRegion& _region, uint32_t _mipCount ) {
        return;
    }
    
    void TextureMTL::release() {
        delete this;
        return;
    }
    
}
