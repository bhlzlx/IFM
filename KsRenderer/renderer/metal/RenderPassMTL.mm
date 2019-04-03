#include "RenderPassMTL.h"
#include "ContextMTL.h"
#include "TextureMTL.h"
#include "MTLTypeMapping.h"

namespace Ks {
    IAttachment* ContextMTL::createAttachment(KsFormat _format) {
        AttachmentMTL* attachment = new AttachmentMTL( _format );
        return attachment;
    }
    
    IRenderPass* ContextMTL::createRenderPass(const RenderPassDescription &_desc, IAttachment **_colorAttachments, IAttachment *_depthStencil) {
        return RenderPassMTL::createRenderPass(_desc, _colorAttachments, _depthStencil);
    }
    
    RenderPassMTL* RenderPassMTL::createRenderPass(const RenderPassDescription &_desc, IAttachment **_colorAttachments, IAttachment *_depthStencil) {
        RenderPassMTL* renderPass = new RenderPassMTL();
        renderPass->m_desc = _desc;
        for( uint32_t i = 0; i<_desc.colorAttachmentCount; ++i) {
            renderPass->m_colorAttachment[i] = (AttachmentMTL*)_colorAttachments[i];
        }
        renderPass->m_depthStencil = (AttachmentMTL*)_depthStencil;
        renderPass->m_clearDepth = 1.0;
        renderPass->m_clearStencil = 0xff;
        renderPass->m_size = {0 , 0};
        return renderPass;
    }
    
    void AttachmentMTL::resize( uint32_t _width, uint32_t _height ) {
        if( m_size.width != _width || m_size.height != _height ) {
            GetContext(context);
            if( m_texture ) {
                m_texture->release();
            }
            TextureDescription desc; {
                desc.format = m_format;
                desc.width  = _width;
                desc.height = _height;
                desc.depth = 1;
                desc.mipmapLevel = 1;
                desc.type = TextureType::Texture2D;
            }
            m_texture = (TextureMTL*)context->createTexture(desc);
        }
    }
    
    const ITexture* AttachmentMTL::getTexture() const {
        return m_texture;
    }
    
    void AttachmentMTL::release() {
        m_texture->release();
    }
    
    KsFormat AttachmentMTL::getFormat() const {
        return m_format;
    }
    
    bool RenderPassMTL::begin(uint32_t _width, uint32_t _height) {
        if( m_size.width != _width || m_size.height != _height ) {
            m_renderPassDescriptor = [MTLRenderPassDescriptor renderPassDescriptor];
            for( uint32_t i = 0; i< m_desc.colorAttachmentCount; ++i ) {
                m_colorAttachment[i]->resize(_width, _height);
                
                m_renderPassDescriptor.colorAttachments[i].loadAction = LoadActionToMTL( m_desc.colorAttachment[i].loadAction );
                m_renderPassDescriptor.colorAttachments[i].texture = ((TextureMTL*)m_colorAttachment[i]->getTexture())->texture();
            }
            //
            m_depthStencil->resize( _width, _height);
            m_renderPassDescriptor.depthAttachment.texture = ((TextureMTL*)m_depthStencil->getTexture())->texture();
            m_renderPassDescriptor.depthAttachment.loadAction = LoadActionToMTL( m_desc.depthStencil.loadAction );
            if( IsStencilFormat( ((TextureMTL*)m_depthStencil->getTexture())->getDesc().format )) {
                m_renderPassDescriptor.stencilAttachment.texture = ((TextureMTL*)m_depthStencil->getTexture())->texture();
                m_renderPassDescriptor.stencilAttachment.loadAction = LoadActionToMTL( m_desc.depthStencil.loadAction );
            }
            m_size = { _width, _height };
        }
        for( uint32_t i = 0; i< m_desc.colorAttachmentCount; ++i ) {
            m_renderPassDescriptor.colorAttachments[i].clearColor = m_clearColors[i];
        }
        m_renderPassDescriptor.depthAttachment.clearDepth = m_clearDepth;
        m_renderPassDescriptor.stencilAttachment.clearStencil = m_clearStencil;
        //
        GetContext(context);
        m_encoder = [context->getCommandBuffer() renderCommandEncoderWithDescriptor:m_renderPassDescriptor];
        context->setRenderCommandEncoder( m_encoder );
        return true;
    }
    
    void RenderPassMTL::end() {
        [m_encoder endEncoding];
        m_encoder = nil;
        GetContext(context);
        context->setRenderCommandEncoder(nil);
    }
    
    void RenderPassMTL::release() {
        delete this;
    }
    
    void RenderPassMTL::setClear(const Ks::RpClear &_clear) {
        for( uint32_t i = 0; i< m_desc.colorAttachmentCount; ++i) {
            m_clearColors[i] = MTLClearColorMake( _clear.colors[i].r, _clear.colors[i].g, _clear.colors[i].b, _clear.colors[i].a );
        }
        m_clearDepth = _clear.depth;
        m_clearStencil = _clear.stencil;
    }
    
    void RenderPassSwapchain::initialize( SwapchainMTL* _swapchain ) {
        m_swapchain = _swapchain;
        m_size = { 0, 0 };
        m_renderPassDescriptor = nil;
        m_encoder = nil;
        m_depthStencil = nil;
        m_desc.colorAttachmentCount = 1;
        m_desc.colorAttachment[0].format = KsBGRA8888_UNORM;
        m_desc.colorAttachment[0].loadAction = RTLoadAction::Clear;
        m_desc.depthStencil.loadAction = RTLoadAction::Clear;
        m_desc.depthStencil.format = KsFormat::KsDepth32F;
    }
    
    bool RenderPassSwapchain::begin( uint32_t _width, uint32_t _height ) {
        id<CAMetalDrawable> drawable = m_swapchain->getCurrentDrawable();
        GetContext( context );
        if( !drawable ) {
            return false;
        }
        
        if( nil == m_renderPassDescriptor ) {
            m_renderPassDescriptor = [MTLRenderPassDescriptor renderPassDescriptor];
            m_renderPassDescriptor.colorAttachments[0].texture = nil;
            m_renderPassDescriptor.colorAttachments[0].loadAction = LoadActionToMTL( m_desc.colorAttachment[0].loadAction );
            //m_renderPassDescriptor.colorAttachments[0].clearColor
            m_renderPassDescriptor.depthAttachment.loadAction = LoadActionToMTL( m_desc.depthStencil.loadAction );
        }
        
        if( m_size.width != _width || m_size.height != _height ) {
            MTLTextureDescriptor* td = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatDepth32Float width:_width height:_height mipmapped:NO];
            td.resourceOptions = MTLResourceStorageModePrivate;
            td.usage = MTLTextureUsageRenderTarget | MTLTextureUsageShaderRead | MTLTextureUsageShaderWrite;
            m_depthStencil = [context->getDevice() newTextureWithDescriptor:td];
            m_renderPassDescriptor.depthAttachment.texture = m_depthStencil;
            m_size = { _width, _height };
        }
        
        m_renderPassDescriptor.colorAttachments[0].texture = drawable.texture;
        m_renderPassDescriptor.colorAttachments[0].clearColor = m_clearColor;
        m_renderPassDescriptor.depthAttachment.clearDepth = m_clearDepth;
        //
        m_encoder = [context->getCommandBuffer() renderCommandEncoderWithDescriptor:m_renderPassDescriptor];
        context->setRenderCommandEncoder(m_encoder);
        return true;
    }
    
    void RenderPassSwapchain::end() {
        [m_encoder endEncoding];
        m_encoder = nil;
        GetContext(context)
        context->setRenderCommandEncoder(nil);
    }
    
    void RenderPassSwapchain::release() {
        delete this;
    }
    
    void RenderPassSwapchain::setClear( const RpClear& _clear ) {
        m_clearColor = MTLClearColorMake( _clear.colors[0].r, _clear.colors[0].g, _clear.colors[0].b, _clear.colors[0].a );
        m_clearDepth = _clear.depth;
    }
}
