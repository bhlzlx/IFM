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
        return false;
    }
    IView* ContextMTL::createView(void* _nativeWindow) {
        return nullptr;
    }
    IStaticVertexBuffer* ContextMTL::createStaticVertexBuffer( void* _data, size_t _size ) {
        return nullptr;
    }
    IDynamicVertexBuffer* ContextMTL::createDynamicVertexBuffer( size_t _size ) {
        return nullptr;
    }
    IIndexBuffer* ContextMTL::createIndexBuffer( void* _data, size_t _size ) {
        return nullptr;
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
    ////////////////////
    ContextMTL ContextMetal;
    IContext* GetContextMetal() {
        return &ContextMetal;
    }
}
