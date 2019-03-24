//
//  QueueMTL.cpp
//  RendererMTL
//
//  Created by kusugawa on 2019/3/22.
//  Copyright © 2019年 kusugawa. All rights reserved.
//

#include "QueueMTL.h"
#include "ContextMTL.h"
#include "BufferMTL.h"
#include "ViewMTL.h"
#include "MTLTypeMapping.mm"
#include <stdint.h>

namespace Ks {
    //
    id<MTLCommandBuffer> GraphicsQueue::getRenderCommandBuffer() {
        return m_renderBuffer;
    }
    
    bool GraphicsQueue::enterFrame() {
        //
        dispatch_semaphore_wait( m_semaphore, -1 );
        //
        m_drawable = m_metalLayer.nextDrawable;
        if( !m_drawable ) {
            return false;
        }
        if( m_transferBuffer ) {
            [m_transferBuffer commit];
            m_transferBuffer = nil;
        }
        m_renderBuffer = [m_queue commandBuffer];
        //
        return true;
    }
    
    void GraphicsQueue::leaveFrame() {
        [m_renderBuffer commit];
        //ContextMTL* context = (ContextMTL*)GetContextMetal();
        [m_renderBuffer commit];
        id<MTLDrawable> drawable = nil;
        [m_renderBuffer presentDrawable:drawable];
        // add complete handler
        [m_renderBuffer addCompletedHandler:^(id<MTLCommandBuffer> _Nonnull) {
            dispatch_semaphore_signal(m_semaphore);
        }];
        m_renderBuffer = nil;
    }
    
    void GraphicsQueue::updateTexture( id<MTLTexture> _texture, const void * _data, size_t _length, const ImageRegion& _region ) {
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
        if( m_renderEncoder ) {
            assert(false);
            return ;
        }
        m_blitEncoder = [command blitCommandEncoder];
        BufferMTL buffer = BufferMTL::createBuffer( _length, _data);
        MTLPixelFormat pixelFormat = _texture.pixelFormat;
        uint32_t bytesperRow = PixelBytes( MTLFormatToKs(pixelFormat) ) * _region.size.width;
        uint32_t bytesperImage = bytesperRow * _region.size.height;
        MTLSize sourcePixel = {
            _region.size.width, _region.size.height, _region.size.depth
        };
        MTLOrigin org;
        org.x = _region.offset.x;
        org.y = _region.offset.y;
        org.z = 0;
        
        [m_blitEncoder copyFromBuffer:buffer.buffer()
                         sourceOffset:0
                    sourceBytesPerRow:bytesperRow
                  sourceBytesPerImage:bytesperImage
                           sourceSize:sourcePixel
                            toTexture:_texture
                     destinationSlice:_region.offset.z
                     destinationLevel:_region.baseMipLevel
                    destinationOrigin:org];
        [m_blitEncoder endEncoding];
        m_blitEncoder = nil;
    }
    
    void GraphicsQueue::updateBuffer( id<MTLBuffer> _buffer, size_t _offset, const void * _data, size_t _length  ) {
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
        if( m_renderEncoder ) {
            assert(false);
            return ;
        }
        m_blitEncoder = [command blitCommandEncoder];
        BufferMTL buffer = BufferMTL::createBuffer( _length, _data);
        [m_blitEncoder copyFromBuffer:buffer.buffer() sourceOffset: 0 toBuffer: _buffer destinationOffset:_offset size:_length];
        [m_blitEncoder endEncoding];
        m_blitEncoder = nil;
    }
}
