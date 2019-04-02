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
        // shader reflection
        MTLRenderPipelineReflection* m_reflection;
    public:
        PipelineMTL(){
        }        
        virtual void begin() override;
        virtual void end() override;
        virtual void setViewport(const Viewport& _viewport) override;
        virtual void setScissor(const Scissor& _scissor) override;
        virtual void draw( IDrawable* _drawable, TopologyMode _pmode, uint32_t _offset, uint32_t _verticesCount ) override;
        virtual void drawIndexed(IDrawable* _drawable, TopologyMode _pmode, uint32_t _indicesCount ) override;
        virtual void drawIndexed(IDrawable* _drawable, IIndexBuffer* _indexBuffer, TopologyMode _pmode, uint32_t _indicesCount) override;
        virtual void drawIndexedInstanced(IDrawable* _drawable, TopologyMode _pmode, uint32_t _indicesCount, uint32_t _instanceCount) override;
        virtual void setPolygonOffset(float _constantBias, float _slopeScaleBias) override;
        virtual IArgument* createArgument(const ArgumentDescriptionOGL& _desc) override;
        virtual IArgument* createArgument(uint32_t _setId) override;
        virtual IDrawable* createDrawable( const DrawableDescription& ) override;
        virtual const PipelineDescription& getDescription() override;
        virtual void release() override;
    };
}
