//
//  DrawableMTL.hpp
//  KsRenderer
//
//  Created by kusugawa on 2019/4/2.
//  Copyright © 2019年 kusugawa. All rights reserved.
//

#pragma once
#include "MetalInc.h"
#include <vector>

namespace Ks {
    
    class PipelineMTL;
    class DrawableMTL : IDrawable {
        friend class PipelineMTL;
    private:
        PipelineMTL * m_pipeline;
        std::vector<id<MTLBuffer>> m_vbo;
        id<MTLBuffer> m_ibo;
        
    public:
        DrawableMTL(){
        }
        ~DrawableMTL(){
        }
        void bind( id<MTLRenderCommandEncoder> _encoder );
        
        id<MTLBuffer> indexBuffer(){
            return m_ibo;
        }
        virtual IPipeline* getPipeline() override ;
        virtual void release() override {
            delete this;
        }
    };
    
}
