//
//  ArgumentMTL.hpp
//  KsRenderer
//
//  Created by kusugawa on 2019/4/1.
//  Copyright © 2019年 kusugawa. All rights reserved.
//

#pragma once
#include "MetalInc.h"
#include <vector>

namespace Ks {
    
    class PipelineMTL;
    class ArgumentMTL : public IArgument {
        friend class PipelineMTL;
    private:
        PipelineMTL* m_pipeline;
        struct SamplerItem{
            MTLFunctionType type;
            uint32_t samplerIndex;
            //uint32_t textureIndex = samplerIndex + 1;
            id<MTLSamplerState> samplerState;
            id<MTLTexture> texture;
        };
        struct UniformItem {
            MTLFunctionType type;
            uint32_t uniformIndex;
            id<MTLBuffer> buffer;
            uint32_t offset;
            uint32_t size;
        };
        //
        std::vector< SamplerItem > m_vecSamplers;
        std::vector< UniformItem > m_vecUniforms;
    public:
        virtual UniformSlot getUniformSlot(const char * _name) override;
        virtual SamplerSlot getSamplerSlot(const char * _name) override;
        virtual void setSampler(SamplerSlot _slot, const SamplerState& _sampler, const ITexture* _texture) override;
        virtual void setUniform(UniformSlot _slot, const void * _data, size_t _size) override;
        virtual void bind() override;
        virtual void release() override;
    };
    
}
