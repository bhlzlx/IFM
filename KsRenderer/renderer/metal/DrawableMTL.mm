//
//  DrawableMTL.cpp
//  KsRenderer
//
//  Created by kusugawa on 2019/4/2.
//  Copyright © 2019年 kusugawa. All rights reserved.
//

#include "DrawableMTL.h"
#include "PipelineMTL.h"

namespace Ks {
    
    void DrawableMTL::bind( id<MTLRenderCommandEncoder> _encoder ) {
        for( size_t i = 0; i<m_vbo.size(); ++i){
            [_encoder setVertexBuffer: m_vbo[i] offset:0 atIndex:i];
        }
    }
    
    IPipeline* DrawableMTL::getPipeline() {
        return m_pipeline;
    }
}
