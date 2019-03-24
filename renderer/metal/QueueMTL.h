#pragma once
#include "MetalInc.h"

namespace Ks {
    
    class GraphicsQueue {
    private:
        id<MTLCommandQueue> m_queue;
        id<MTLCommandBuffer> m_transferBuffer;
        id<MTLCommandBuffer> m_renderBuffer;
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
        void enterFrame();
        void leaveFrame();
        //
        void updateTexture( id<MTLTexture> _texture );
        void updateBuffer( id<MTLBuffer> _buffer );
    };
    /*
    class UploadQueue {
    private:
        id<MTLCommandQueue> m_queue;
    public:
        UploadQueue() {
        }
    };*/
}
