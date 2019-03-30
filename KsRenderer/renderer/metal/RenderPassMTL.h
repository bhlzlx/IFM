#pragma once
#include <KsRenderer.h>
#include "MetalInc.h"

namespace Ks
{
    class TextureMTL;
    class SwapchainMTL;
    
    class AttachmentMTL: public IAttachment {
    private:
        KsFormat m_format;
        TextureMTL* m_texture;
        Size<uint32_t> m_size;
    public:
        void resize( uint32_t _width, uint32_t _height ) override;
        const ITexture* getTexture() const override;
        void release() override;
        KsFormat getFormat() const override;
    };
    
    class RenderPassMTL : public IRenderPass {
    private:
        RenderPassDescription m_desc;
        Size<uint32_t> m_size;
        //
        MTLRenderPassDescriptor* m_renderPassDescriptor;
        id<MTLRenderCommandEncoder> m_encoder;
        //
        AttachmentMTL* m_colorAttachment[MaxRenderTarget];
        AttachmentMTL* m_depthStencil;
        
        MTLClearColor m_clearColors[MaxRenderTarget];
        float m_clearDepth;
        uint8_t m_clearStencil;
    public:
        RenderPassMTL() {
        }
        
        bool begin(uint32_t _width, uint32_t _height) override;
        
        void end() override;
        
        void release() override;
        
        void setClear(const Ks::RpClear &_clear) override;
        
    };
    
    class RenderPassSwapchain : public IRenderPass {
    private:
        RenderPassDescription m_desc;
        SwapchainMTL* m_swapchain;
        MTLRenderPassDescriptor* m_renderPassDescriptor;
        id<MTLRenderCommandEncoder> m_encoder;
        id<MTLTexture> m_depthStencil;
        //
        MTLClearColor m_clearColor;
        float m_clearDepth;
        //
        Size<uint32_t> m_size;
    public:
        RenderPassSwapchain() {
        }
        //
        void initialize( SwapchainMTL* _swapchain );
        virtual bool begin( uint32_t _width, uint32_t _height ) override;
        virtual void end() override;
        virtual void release() override;
        virtual void setClear( const RpClear& _clear ) override;
    };
}
