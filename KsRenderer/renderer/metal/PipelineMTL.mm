//
//  PipelineMTL.cpp
//  KsRenderer
//
//  Created by kusugawa on 2019/3/31.
//  Copyright © 2019年 kusugawa. All rights reserved.
//

#include "PipelineMTL.h"
#include "ContextMTL.h"
#include "ShaderModuleMTL.h"
#include "MTLTypeMapping.h"
#include "ks/io/io.h"

namespace Ks {
    
    IPipeline* ContextMTL::createPipeline( const PipelineDescription& _desc ) {
        // goal :
        // 1. create `render pipeline state, depth stencil state` ( completed )
        // 2. create argument template information ( not implemented yet! )
        bool rst = false;
        // shader module from file
        TextReader vertReader,fragReader;
        rst = vertReader.openFile( m_archieve, _desc.vertexShader);
        assert(rst);
        rst = fragReader.openFile( m_archieve, _desc.fragmentShader);
        assert(rst);
        ShaderModuleMTL vertModule, fragModule;
        rst = ShaderModuleMTL::createShaderModule( vertReader.getText(), MTLFunctionTypeVertex, vertModule);
        assert(rst);
        rst = ShaderModuleMTL::createShaderModule( fragReader.getText(), MTLFunctionTypeFragment, fragModule);
        assert(rst);
        //
        MTLRenderPipelineDescriptor* descriptor = [[MTLRenderPipelineDescriptor alloc] init];
        // shader
        descriptor.vertexFunction = vertModule.shader();
        descriptor.fragmentFunction = fragModule.shader();
        // vertex layer
        MTLVertexDescriptor* vertexDescriptor = [MTLVertexDescriptor vertexDescriptor];
        for( uint32_t vertIndex = 0; vertIndex<_desc.vertexLayout.vertexAttributeCount; ++vertIndex) {
            vertexDescriptor.attributes[vertIndex].format = VertexFormatToMTL( _desc.vertexLayout.vertexAttributes[vertIndex].type);
            vertexDescriptor.attributes[vertIndex].bufferIndex = _desc.vertexLayout.vertexAttributes[vertIndex].bufferIndex;
            vertexDescriptor.attributes[vertIndex].offset = _desc.vertexLayout.vertexAttributes[vertIndex].offset;
        }
        for( uint32_t bufferIndex = 0; bufferIndex<_desc.vertexLayout.vertexBufferCount; ++bufferIndex){
            vertexDescriptor.layouts[bufferIndex].stride = _desc.vertexLayout.vertexBuffers[bufferIndex].stride;
            vertexDescriptor.layouts[bufferIndex].stepRate = _desc.vertexLayout.vertexBuffers[bufferIndex].rate;
            vertexDescriptor.layouts[bufferIndex].stepFunction = _desc.vertexLayout.vertexBuffers[bufferIndex].instanceMode ? MTLVertexStepFunctionPerInstance : MTLVertexStepFunctionPerVertex;
        }
        //descriptor.vertexBuffers;
        //descriptor.fragmentBuffers;
        // render pass descriptioin
        for ( uint32_t colorIndex = 0; colorIndex < _desc.renderPassDescription.colorAttachmentCount; ++colorIndex) {
            descriptor.colorAttachments[colorIndex].pixelFormat = KsFormatToMTL( _desc.renderPassDescription.colorAttachment[colorIndex].format );
            descriptor.colorAttachments[colorIndex].blendingEnabled = _desc.renderState.blendEnable;
            descriptor.colorAttachments[colorIndex].sourceRGBBlendFactor = BlendFactorToMTL( _desc.renderState.blendSource );
            descriptor.colorAttachments[colorIndex].destinationRGBBlendFactor = BlendFactorToMTL( _desc.renderState.blendDestination );
            descriptor.colorAttachments[colorIndex].rgbBlendOperation = BlendOpToMTL( _desc.renderState.blendOperation );
            descriptor.colorAttachments[colorIndex].sourceAlphaBlendFactor = BlendFactorToMTL( _desc.renderState.blendSource );
            descriptor.colorAttachments[colorIndex].destinationAlphaBlendFactor = BlendFactorToMTL( _desc.renderState.blendDestination );
            descriptor.colorAttachments[colorIndex].alphaBlendOperation = BlendOpToMTL( _desc.renderState.blendOperation );;
            descriptor.colorAttachments[colorIndex].writeMask = 0;
            if( _desc.renderState.red )
                descriptor.colorAttachments[colorIndex].writeMask |= MTLColorWriteMaskRed;
            if( _desc.renderState.green )
                descriptor.colorAttachments[colorIndex].writeMask |= MTLColorWriteMaskGreen;
            if( _desc.renderState.blue )
                descriptor.colorAttachments[colorIndex].writeMask |= MTLColorWriteMaskBlue;
            if( _desc.renderState.alpha )
                descriptor.colorAttachments[colorIndex].writeMask |= MTLColorWriteMaskAlpha;
        }
        if( _desc.renderPassDescription.depthStencil.format != KsInvalidFormat ){
            descriptor.depthAttachmentPixelFormat = KsFormatToMTL( _desc.renderPassDescription.depthStencil.format );
            if( IsStencilFormat( _desc.renderPassDescription.depthStencil.format )){
                descriptor.stencilAttachmentPixelFormat = descriptor.depthAttachmentPixelFormat;
            }
        }
        // render state
        descriptor.sampleCount = 1;
        descriptor.inputPrimitiveTopology = MTLPrimitiveTopologyClassTriangle;
        //
        MTLDepthStencilDescriptor* depthStencilDescriptor = [[MTLDepthStencilDescriptor alloc] init];
        depthStencilDescriptor.depthCompareFunction = CompareFunctionToMTL(_desc.renderState.depthFunction);
        depthStencilDescriptor.depthWriteEnabled = _desc.renderState.depthWriteEnable;
        depthStencilDescriptor.frontFaceStencil.writeMask = depthStencilDescriptor.frontFaceStencil.readMask = _desc.renderState.stencilMask;
        depthStencilDescriptor.frontFaceStencil.stencilCompareFunction = CompareFunctionToMTL( _desc.renderState.stencilFunction );
        depthStencilDescriptor.frontFaceStencil.stencilFailureOperation = StencilOpToMTL( _desc.renderState.stencilFail );
        depthStencilDescriptor.frontFaceStencil.depthStencilPassOperation = StencilOpToMTL( _desc.renderState.stencilPass );
        //
        depthStencilDescriptor.backFaceStencil.writeMask = depthStencilDescriptor.backFaceStencil.readMask = _desc.renderState.stencilMask;
        depthStencilDescriptor.backFaceStencil.stencilCompareFunction = CompareFunctionToMTL( _desc.renderState.stencilFunction );
        depthStencilDescriptor.backFaceStencil.stencilFailureOperation = StencilOpToMTL( _desc.renderState.stencilFailCCW );
        depthStencilDescriptor.backFaceStencil.depthStencilPassOperation = StencilOpToMTL( _desc.renderState.stencilPassCCW );
        
        NSError* pipelinError = nil;
        //
        id<MTLRenderPipelineState> pipelineState = [m_device newRenderPipelineStateWithDescriptor:descriptor error:&pipelinError];
        id<MTLDepthStencilState> depthStencilState = [m_device newDepthStencilStateWithDescriptor:depthStencilDescriptor];
        
        // to be continued...
    }
    
    void PipelineMTL::begin(){
        
    }
    void PipelineMTL::end(){
        
    }
    void PipelineMTL::setViewport(const Viewport& _viewport){

    }
    void PipelineMTL::setScissor(const Scissor& _scissor){

    }
    void PipelineMTL::draw( IDrawable* _drawable, TopologyMode _pmode, uint32_t _offset, uint32_t _verticesCount ){

    }
    void PipelineMTL::drawIndexed(IDrawable* _drawable, TopologyMode _pmode, uint32_t _indicesCount ){

    }
    void PipelineMTL::drawIndexed(IDrawable* _drawable, IIndexBuffer* _indexBuffer, TopologyMode _pmode, uint32_t _indicesCount){

    }
    void PipelineMTL::drawIndexedInstanced(IDrawable* _drawable, TopologyMode _pmode, uint32_t _indicesCount, uint32_t _instanceCount ) {

    }
    void PipelineMTL::setPolygonOffset(float _constantBias, float _slopeScaleBias){

    }
    IArgument* PipelineMTL::createArgument(const ArgumentDescriptionOGL& _desc){

    }
    IArgument* PipelineMTL::createArgument(uint32_t _setId){

    }
    IDrawable* PipelineMTL::createDrawable( const DrawableDescription& ){

    }
    const PipelineDescription& PipelineMTL::getDescription(){

    }
    void PipelineMTL::release(){

    }
}
