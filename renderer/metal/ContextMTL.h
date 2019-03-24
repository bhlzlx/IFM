//
//  ContextMTL.hpp
//  RendererMTL
//
//  Created by kusugawa on 2019/3/22.
//  Copyright © 2019年 kusugawa. All rights reserved.
//
#pragma once
#include <KsRenderer.h>
#include "MetalInc.h"
#include "QueueMTL.h"

#define GetContext( _ctx ) auto _ctx = (ContextMTL*)GetContextMetal();

namespace Ks {
    
    class ViewMTL;
    
    class ContextMTL : public IContext {
        id<MTLDevice> m_device;
        CAMetalLayer* m_metalLayer;
        GraphicsQueue m_graphicsQueue;
    public:
        virtual bool initialize( Ks::IArch* _arch, void* _wnd ) override;
        virtual IView* createView(void* _nativeWindow) override;
        virtual IStaticVertexBuffer* createStaticVertexBuffer( void* _data, size_t _size ) override;
        virtual IDynamicVertexBuffer* createDynamicVertexBuffer( size_t _size ) override;
        virtual IIndexBuffer* createIndexBuffer( void* _data, size_t _size ) override;
        virtual ITexture* createTexture(const TextureDescription& _desc, TextureUsageFlags _usage = TextureUsageNone ) override;
        virtual IAttachment* createAttachment(KsFormat _format) override;
        virtual IRenderPass* createRenderPass( const RenderPassDescription& _desc, IAttachment** _colorAttachments, IAttachment* _depthStencil ) override;
        virtual IPipeline* createPipeline( const PipelineDescription& _desc ) override;
        virtual KsFormat swapchainFormat() const override;
        //
        void present();
        //
        GraphicsQueue& getGraphicsQueue() {
            return m_graphicsQueue;
        }
        //
        id<MTLCommandBuffer> getCommandBuffer() {
            return m_graphicsQueue.getRenderCommandBuffer();
        }
        id<MTLDevice> getDevice() {
            return m_device;
        }
    };
}
