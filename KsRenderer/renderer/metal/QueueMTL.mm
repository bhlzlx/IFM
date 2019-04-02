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
#include "TextureMTL.h"
#include "ViewMTL.h"
#include "SwapchainMTL.h"
#include "MTLTypeMapping.h"
#include <stdint.h>

namespace Ks {
    //
    id<MTLCommandBuffer> GraphicsQueue::getRenderCommandBuffer() {
        return m_renderBuffer;
    }
    
    void GraphicsQueue::initialize( id<MTLCommandQueue> _queue, SwapchainMTL* _swapchain ) {
        m_queue = _queue;
        m_swapchain = _swapchain;
    }
    
    void UploadQueue::initialize( id<MTLCommandQueue> _queue ) {
        m_queue = _queue;
        m_semaphore = dispatch_semaphore_create(0);
    }
    
    bool GraphicsQueue::enterFrame() {
        if( !m_swapchain->prepareFrame() ){
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
        id<CAMetalDrawable> drawable = m_swapchain->getCurrentDrawable();
        auto semaphore = m_swapchain->getSemaphore();
        // add complete handler
        [m_renderBuffer addCompletedHandler:^(id<MTLCommandBuffer> _Nonnull) {
            dispatch_semaphore_signal(semaphore);
            [drawable present];
        }];
        [m_renderBuffer commit];
        m_renderBuffer = nil;
        m_flightIndex = (m_flightIndex + 1 )%3;
    }

    //void updateTexture( TextureMTL* _texture, const void * _data, size_t _length, const TextureRegion& _region, uint32_t _mipCount );
    
    void GraphicsQueue::updateTexture( TextureMTL* _texture, const void * _data, size_t _length, const TextureRegion& _region ) {
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
#ifndef NDEBUG
        GetContext(context)
        assert( context->getRenderCommandEncoder() == nil );
#endif
        m_blitEncoder = [command blitCommandEncoder];
        BufferMTL* buffer = BufferMTL::createBuffer( _length, _data, BufferUsageUniform );
        MTLPixelFormat pixelFormat = _texture->texture().pixelFormat;
        uint32_t bytesperRow = PixelBits( MTLFormatToKs(pixelFormat) ) * _region.size.width / 8;
        uint32_t bytesperImage = bytesperRow * _region.size.height;
        MTLSize sourcePixel = {
            _region.size.width, _region.size.height, _region.size.depth
        };
        MTLOrigin org;
        org.x = _region.offset.x;
        org.y = _region.offset.y;
        org.z = 0;
        
        [m_blitEncoder copyFromBuffer:buffer->buffer()
                         sourceOffset:0
                    sourceBytesPerRow:bytesperRow
                  sourceBytesPerImage:bytesperImage
                           sourceSize:sourcePixel
                            toTexture:_texture->texture()
                     destinationSlice:_region.baseLayer
                     destinationLevel:_region.mipLevel
                    destinationOrigin:org];
        [m_blitEncoder endEncoding];
        m_blitEncoder = nil;
        buffer->release();
    }
    
    void GraphicsQueue::updateBuffer( BufferMTL* _buffer, size_t _offset, const void * _data, size_t _length ) {
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
#ifndef NDEBUG
        GetContext(context)
        assert( context->getRenderCommandEncoder() == nil );
#endif
        m_blitEncoder = [command blitCommandEncoder];
        BufferMTL* buffer = BufferMTL::createBuffer( _length, _data, BufferUsageUniform );
        buffer->setDataBlocked( _data, _length, 0);
        [m_blitEncoder copyFromBuffer:buffer->buffer() sourceOffset: 0 toBuffer: _buffer->buffer() destinationOffset:_offset size:_length];
        [m_blitEncoder endEncoding];
        m_blitEncoder = nil;
        buffer->release();
    }
    
    void UploadQueue::uploadBuffer( BufferMTL* _buffer, size_t _offset, const void * _data, size_t _length ) {
        id<MTLCommandBuffer> cmd = [m_queue commandBuffer];
        id<MTLBlitCommandEncoder> encoder = [cmd blitCommandEncoder];
        BufferMTL* stagingBuffer = BufferMTL::createBuffer( _length, _data, BufferUsageUniform );
        stagingBuffer->setDataBlocked( _data, _length, 0);
        [encoder copyFromBuffer:stagingBuffer->buffer() sourceOffset: 0 toBuffer: _buffer->buffer() destinationOffset:_offset size:_length];
        [encoder endEncoding];
        [cmd addCompletedHandler:^(id<MTLCommandBuffer> _Nonnull) {
            dispatch_semaphore_signal( m_semaphore );
        }];
        [cmd commit];
        dispatch_semaphore_wait( m_semaphore, -1 );
        stagingBuffer->release();
    }
    
    void UploadQueue::uploadTexture( TextureMTL* _texture, const void * _data, size_t _length, const TextureRegion& _region ) {
        id<MTLCommandBuffer> cmd = [m_queue commandBuffer];
        id<MTLBlitCommandEncoder> encoder = [cmd blitCommandEncoder];
        BufferMTL* stagingBuffer = BufferMTL::createBuffer( _length, _data, BufferUsageUniform );
        stagingBuffer->setDataBlocked( _data, _length, 0);
        //
        MTLPixelFormat pixelFormat = _texture->texture().pixelFormat;
        uint32_t bytesperRow = PixelBits( MTLFormatToKs(pixelFormat) ) * _region.size.width / 8;
        uint32_t bytesperImage = bytesperRow * _region.size.height;
        MTLSize sourcePixel = {
            _region.size.width, _region.size.height, _region.size.depth
        };
        MTLOrigin org;
        org.x = _region.offset.x;
        org.y = _region.offset.y;
        org.z = 0;
        
        [encoder copyFromBuffer:stagingBuffer->buffer()
                         sourceOffset:0
                    sourceBytesPerRow:bytesperRow
                  sourceBytesPerImage:bytesperImage
                           sourceSize:sourcePixel
                            toTexture:_texture->texture()
                     destinationSlice:_region.baseLayer
                     destinationLevel:_region.mipLevel
                    destinationOrigin:org];
        [encoder endEncoding];
        
        [cmd addCompletedHandler:^(id<MTLCommandBuffer> _Nonnull) {
            dispatch_semaphore_signal( m_semaphore );
        }];
        [cmd commit];
        dispatch_semaphore_wait( m_semaphore, -1 );
        stagingBuffer->release();
    }
}
