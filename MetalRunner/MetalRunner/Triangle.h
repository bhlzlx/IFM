//
//  Triangle.hpp
//  MetalRunner
//
//  Created by pixelsoft on 2019/4/3.
//  Copyright © 2019 kusugawa. All rights reserved.
//

#include <KsRenderer/KsApplication.h>
#include <KsRenderer/KsRenderer.h>
#include "Ks/io/io.h"

namespace Ks {
    
    class Triangle : public KsApplication {
    private:
        IContext* m_context;
        IView* m_view;
        IPipeline* m_pipeline;
        //
        IStaticVertexBuffer* m_vbo;
        IIndexBuffer* m_ibo;
        ITexture* m_texture;
        //
        IArgument* m_globalArgument;
        IArgument* m_localArgument;
        IDrawable* m_drawable;
        //
        SamplerSlot m_samBaseSlot;
        UniformSlot m_globalUBOSlot;
        UniformSlot m_localUBOSlot;
        //
        ArchieveProtocol* m_archieve;
        //
        Size<uint32_t> m_size;
        Viewport m_viewport;
        Scissor m_scissor;
        bool m_resInited;
    public:
        virtual bool initialize(void* _wnd, Ks::ArchieveProtocol* _archieve) override;
        void initializeResources();
        virtual void resize(uint32_t _width, uint32_t _height) override;
        virtual void release() override;
        virtual void tick() override;
        virtual const char * title() override {
            return "Triangle";
        }
        virtual DeviceType contextType() override {
            return Metal;
            //return Vulkan;
        }
    };
}
