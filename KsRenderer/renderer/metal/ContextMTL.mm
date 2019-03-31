//
//  ContextMTL.cpp
//  RendererMTL
//
//  Created by kusugawa on 2019/3/22.
//  Copyright © 2019年 kusugawa. All rights reserved.
//

#include "ContextMTL.h"

namespace Ks {
    bool ContextMTL::initialize( Ks::IArch* _arch, void* _wnd ) {
        m_device = MTLCreateSystemDefaultDevice();
        m_metalLayer = (__bridge __weak CAMetalLayer*)(_wnd);
        m_metalLayer.device = m_device;
        m_swapchain.initialize( m_metalLayer );
        //
        id<MTLCommandQueue> graphicsQueue = [m_device newCommandQueue];
        m_graphicsQueue.initialize( graphicsQueue, &m_swapchain );
        id<MTLCommandQueue> uploadQueue = [m_device newCommandQueue];
        m_uploadQueue.initialize( uploadQueue );
        //
        m_mainRenderPass.initialize( &m_swapchain );
        return true;
    }
    
    ITexture* ContextMTL::createTexture(const TextureDescription& _desc, TextureUsageFlags _usage ) {
        return nullptr;
    }
    IAttachment* ContextMTL::createAttachment(KsFormat _format) {
        return nullptr;
    }
    IRenderPass* ContextMTL::createRenderPass( const RenderPassDescription& _desc, IAttachment** _colorAttachments, IAttachment* _depthStencil ) {
        return nullptr;
    }
    IPipeline* ContextMTL::createPipeline( const PipelineDescription& _desc ) {
        return nullptr;
    }
    KsFormat ContextMTL::swapchainFormat() const {
        return KsBGRA8888_UNORM;
    }
    ITexture* ContextMTL::createTextureDDS( const void* _data, size_t _length ) {
        return nullptr;
    }
    ITexture* ContextMTL::createTextureKTX(const void* _data, size_t _length) {
        return nullptr;
    }
    ////////////////////
    ContextMTL ContextMetal;
    IContext* GetContextMetal() {
        return &ContextMetal;
    }
}
