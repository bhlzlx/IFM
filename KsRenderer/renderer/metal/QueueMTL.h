#pragma once
#include "MetalInc.h"

namespace Ks {
    
    class TextureMTL;
    class BufferMTL;
    class SwapchainMTL;
    
    class GraphicsQueue {
    private:
        id<MTLCommandQueue> m_queue;
        id<MTLCommandBuffer> m_transferBuffer;
        id<MTLBlitCommandEncoder> m_blitEncoder;
        id<MTLCommandBuffer> m_renderBuffer;
        //
        SwapchainMTL* m_swapchain;
        uint32_t m_flightIndex;
        //
        bool m_enterFrame;
    public:
        GraphicsQueue() :
        m_queue(nil)
        ,m_transferBuffer(nil)
        ,m_blitEncoder(nil)
        ,m_renderBuffer(nil)
        ,m_swapchain(nil)
        ,m_flightIndex(0)
        ,m_enterFrame(false)
        {
        }
        
        void initialize( id<MTLCommandQueue> _queue, SwapchainMTL* _swapchain );
        id<MTLCommandBuffer> getRenderCommandBuffer();
        //
        bool enterFrame();
        void leaveFrame();
        //
        void updateTexture( TextureMTL* _texture, const void * _data, size_t _length, const TextureRegion& _region );
        void updateTexture( TextureMTL* _texture, const void * _data, size_t _length, const TextureRegion& _region, uint32_t _mipCount );
        //
        void updateBuffer( BufferMTL* _buffer, size_t _offset, const void * _data, size_t _length );
        uint32_t getFlightIndex() {
            return m_flightIndex;
        }
    };
    
    class UploadQueue {
    private:
        id<MTLCommandQueue> m_queue;
        dispatch_semaphore_t m_semaphore;
        //
    public:
        void initialize( id<MTLCommandQueue> _queue );
        void uploadBuffer( BufferMTL* _buffer, size_t _offset, const void * _data, size_t _length );
        void uploadTexture( TextureMTL* _texture, const void * _data, size_t _length, const TextureRegion& _region );
    };
}
