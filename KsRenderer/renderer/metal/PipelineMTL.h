//
//  PipelineMTL.hpp
//  KsRenderer
//
//  Created by kusugawa on 2019/3/31.
//  Copyright © 2019年 kusugawa. All rights reserved.
//
#pragma once
#include "MetalInc.h"
#include "ShaderModuleMTL.h"

namespace Ks {
    
    class ArgumentMTL;
    class PipelineMTL : public IPipeline {
        friend class ContextMTL;
        friend class ArgumentMTL;
    private:
        // description
        PipelineDescription m_desc;
        // core objects
        id<MTLRenderPipelineState> m_pipelineState;
        id<MTLDepthStencilState> m_depthStencilState;
        MTLViewport m_viewport;
        MTLScissorRect m_scissor;
        // shader reflection
        MTLRenderPipelineReflection* m_reflection;
        //
        float m_constantBias;
        float m_slopeScaleBias;
    public:
        PipelineMTL(){
        }        
        virtual void begin() override;
        virtual void end() override;
        virtual void setViewport(const Viewport& _viewport) override;
        virtual void setScissor(const Scissor& _scissor) override;
        virtual void setPolygonOffset(float _constantBias, float _slopeScaleBias) override;
        virtual IArgument* createArgument(const ArgumentDescription& _desc) override;
        virtual IArgument* createArgument(uint32_t _setId) override;
        virtual const PipelineDescription& getDescription() override;
        virtual void release() override;
        
        void bindVertexBuffer(uint32_t _index, Ks::IBuffer *_vertexBuffer, uint32_t _offset) override ;
        
        void draw(Ks::TopologyMode _pmode, uint32_t _offset, uint32_t _vertexCount) override ;
        
        void drawIndexed(Ks::TopologyMode _pmode, Ks::IBuffer *_indexBuffer, uint32_t _indexOffset, uint32_t _indexCount) override;
        
        void drawInstanced(Ks::TopologyMode _pmode, uint32_t _vertexOffset, uint32_t _vertexCount, uint32_t _instanceCount) override ;
        
        void drawInstanced(Ks::TopologyMode _pmode, Ks::IBuffer *_indexBuffer, uint32_t _indexOffset, uint32_t _indexCount, uint32_t _instanceCount) override ;
        
        void setShaderCache(const void *_data, size_t _size, size_t _offset) override;
        
    };
}
