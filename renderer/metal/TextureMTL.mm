#include "TextureMTL.h"
#include "ContextMTL.h"
#include "QueueMTL.h"
#include "MTLTypeMapping.mm"

namespace Ks {
    
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
    
    void TextureMTL::setSubData(const void *_data, size_t _length, const Ks::ImageRegion &_region) {
        // queue update
        GetContext(context);
        auto& queue = context->getGraphicsQueue();
        queue.updateTexture( m_texture, _data, _length, _region);
    }
    
    void TextureMTL::release() {
        delete this;
        return;
    }
    
}
