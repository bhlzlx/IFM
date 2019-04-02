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
#include "SwapchainMTL.h"
#include "RenderPassMTL.h"
#include "UniformMTL.h"

#define GetContext( _ctx ) auto _ctx = (ContextMTL*)GetContextMetal();

namespace Ks {
    
    class ViewMTL;
    
    class ContextMTL : public IContext {
    private:
        id<MTLDevice> m_device;
        Ks::IArch* m_archieve;
        CAMetalLayer* m_metalLayer;
        SwapchainMTL m_swapchain;
        //
        GraphicsQueue m_graphicsQueue; // initialize with swapchain
        UploadQueue m_uploadQueue; // 
        RenderPassSwapchain m_mainRenderPass; // initialize with swapchain
        TransientUniformAllocator m_tubollator[MaxFlightCount];
        //
        id<MTLRenderCommandEncoder> m_renderEncoder;
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
        virtual ITexture* createTextureDDS( const void* _data, size_t _length ) override;
        virtual ITexture* createTextureKTX(const void* _data, size_t _length) override;
        //
        void present();
        //
        GraphicsQueue& getGraphicsQueue() {
            return m_graphicsQueue;
        }
        UploadQueue& getUploadQueue() {
            return m_uploadQueue;
        }
        TransientUniformAllocator& getTUBOAllocator() {
            return m_tubollator[m_graphicsQueue.getFlightIndex()];
        }
        
        RenderPassSwapchain& getMainRenderPass() {
            return m_mainRenderPass;
        }
        
        void setRenderCommandEncoder( id<MTLRenderCommandEncoder> _encoder ) {
            if( nil!=_encoder) {
                assert( m_renderEncoder == nil );
            }
            m_renderEncoder = _encoder;
        }
        
        id<MTLRenderCommandEncoder> getRenderCommandEncoder() {
            return m_renderEncoder;
        }
        
        CAMetalLayer* getMetalLayer() {
            return m_metalLayer;
        }
        //
        id<MTLCommandBuffer> getCommandBuffer() {
            return m_graphicsQueue.getRenderCommandBuffer();
        }
        id<MTLDevice> getDevice() {
            return m_device;
        }
        Ks::IArch* archieve() {
            return m_archieve;
        }
    };
}
