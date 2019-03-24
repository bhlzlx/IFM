#pragma once
#include <KsRenderer.h>
#include "MetalInc.h"

namespace Ks
{
    class TextureMTL;
    
    class AttachmentMTL: public IAttachment {
    private:
        TextureMTL* m_texture;
    public:
    };
    
    class RenderPassMTL : public IRenderPass {
    private:
        AttachmentMTL* m_colorAttachment[MaxRenderTarget];
        AttachmentMTL* m_depthStencil;
    public:
        RenderPassMTL() {
        }
    };
    
    class RenderPassMetalLayer : public IRenderPass {
    private:
        id<MTLDrawable> m_drawable;
    public:
        RenderPassMetalLayer():m_drawable(nil) {
        }
        
        id<MTLDrawable> drawable() {
            return m_drawable;
        }
    };
}
