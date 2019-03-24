//
//  QueueMTL.cpp
//  RendererMTL
//
//  Created by kusugawa on 2019/3/22.
//  Copyright © 2019年 kusugawa. All rights reserved.
//

#include "QueueMTL.h"
#include "ContextMTL.h"

namespace Ks {
    //
    id<MTLCommandBuffer> GraphicsQueue::getRenderCommandBuffer() {
        return m_renderBuffer;
    }
    
    void GraphicsQueue::enterFrame() {
        m_renderBuffer = [m_queue commandBuffer];
    }
    
    void GraphicsQueue::leaveFrame() {
        [m_renderBuffer commit];
        ContextMTL* context = (ContextMTL*)GetContextMetal();
    }
    
    void GraphicsQueue::updateTexture( id<MTLTexture> _texture ) {
        id<MTLCommandBuffer> command = nil;
        if( !m_enterFrame ) {
            if( m_transferBuffer == nil ) {
                m_transferBuffer = [m_queue commandBuffer];
            }
            command = m_transferBuffer;
        }
        else
        {
            command = m_renderBuffer;
        }
        //
        command
    }
    
    void GraphicsQueue::updateBuffer( id<MTLBuffer> _buffer ) {
        
    }
}
