#pragma once
#include "MetalInc.h"

namespace Ks {
    
    class TextureMTL;
    class BufferMTL;
    
    class GraphicsQueue {
    private:
        id<MTLCommandQueue> m_queue;
        id<MTLCommandBuffer> m_transferBuffer;
        id<MTLBlitCommandEncoder> m_blitEncoder;
        id<MTLCommandBuffer> m_renderBuffer;
        id<MTLRenderCommandEncoder> m_renderEncoder;
        //
        CAMetalLayer* m_metalLayer;
        id<MTLDrawable> m_drawable;
        dispatch_semaphore_t m_semaphore;
        bool m_enterFrame;
    public:
        GraphicsQueue() :
        m_queue(nil)
        ,m_transferBuffer(nil)
        ,m_renderBuffer(nil)
        {
        }
        
        void initialize( id<MTLCommandQueue> _queue );
        id<MTLCommandBuffer> getRenderCommandBuffer();
        //
        bool enterFrame();
        void leaveFrame();
        //
        void updateTexture( TextureMTL* _texture, const void * _data, size_t _length, const TextureRegion& _region );
        void updateTexture( TextureMTL* _texture, const void * _data, size_t _length, const TextureRegion& _region, uint32_t _mipCount );
        //
        void updateBuffer( BufferMTL* _buffer, size_t _offset, const void * _data, size_t _length );
    };
}
