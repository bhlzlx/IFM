//
//  ContextMTL.cpp
//  RendererMTL
//
//  Created by kusugawa on 2019/3/22.
//  Copyright © 2019年 kusugawa. All rights reserved.
//

#include "ContextMTL.h"
#include "TextureMTL.h"
#include "RenderPassMTL.h"

namespace Ks {
    bool ContextMTL::initialize( Ks::IArch* _arch, void* _wnd ) {
        m_archieve = _arch;
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
        for( auto & allocator : m_tubollator )
            allocator.initialize();
        return true;
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
