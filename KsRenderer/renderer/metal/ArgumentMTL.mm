//
//  ArgumentMTL.cpp
//  KsRenderer
//
//  Created by kusugawa on 2019/4/1.
//  Copyright © 2019年 kusugawa. All rights reserved.
//

#include "ArgumentMTL.h"
#include "ContextMTL.h"
#include "PipelineMTL.h"
#include "TextureMTL.h"
#include "MTLTypeMapping.h"
#include <map>

namespace Ks {
    
    std::map< SamplerState, id<MTLSamplerState> > SamplerStateTable;
    
    id<MTLSamplerState> GetSamplerObject( const SamplerState& _state ) {
        auto it = SamplerStateTable.find( _state );
        if( it == SamplerStateTable.end()) {
            MTLSamplerDescriptor* ss = [[MTLSamplerDescriptor alloc] init];
            ss.magFilter = MinMagFilterToMTL( _state.mag );
            ss.minFilter = MinMagFilterToMTL( _state.min );
            ss.mipFilter = MipmapFilterToMTL( _state.mip );
            ss.sAddressMode = AddressModeToMTL( _state.u );
            ss.tAddressMode = AddressModeToMTL( _state.v );
            ss.rAddressMode = AddressModeToMTL( _state.w );
            ss.compareFunction = CompareFunctionToMTL( _state.compareFunction );
            //
            GetContext( context )
            id<MTLSamplerState> state = [context->getDevice() newSamplerStateWithDescriptor:ss];
            SamplerStateTable[_state] = state;
            return state;
        }
        return it->second;
    }
    
    UniformSlot ArgumentMTL::getUniformSlot(const char * _name) {
        UniformSlot slot;
        slot.mtlIndex = -1;
        MTLFunctionType type = MTLFunctionTypeVertex;
        uint32_t index = -1;
        for ( MTLArgument* arg in m_pipeline->m_reflection.vertexArguments ) {
            if ([arg.name isEqualToString: [NSString stringWithCString:_name encoding:NSUTF8StringEncoding]] && arg.type == MTLArgumentTypeBuffer) {
                type = MTLFunctionTypeVertex;
                index = (uint32_t)arg.index;
                break;
            }
        }
        if( index == -1 )
        {
            for ( MTLArgument* arg in m_pipeline->m_reflection.fragmentArguments ) {
                if ([arg.name isEqualToString: [NSString stringWithCString:_name encoding:NSUTF8StringEncoding]] && arg.type == MTLArgumentTypeBuffer) {
                    type = MTLFunctionTypeFragment;
                    index = (uint32_t)arg.index;
                    break;
                }
            }
        }
        if( index == -1 ){
            return slot;
        }
        for ( uint32_t i = 0;i <m_vecUniforms.size(); ++i) {
            if ( m_vecUniforms[i].uniformIndex == index && m_vecUniforms[i].type == type ) {
                slot.mtlIndex = i;
                break;
            }
        }
        return slot;
    }
    SamplerSlot ArgumentMTL::getSamplerSlot(const char * _name) {
        SamplerSlot slot;
        slot.mtlIndex = -1;
        MTLFunctionType type = MTLFunctionTypeVertex;
        uint32_t index = -1;
        for ( MTLArgument* arg in m_pipeline->m_reflection.vertexArguments ) {
            if ([arg.name isEqualToString: [NSString stringWithCString:_name encoding:NSUTF8StringEncoding]] && arg.type == MTLArgumentTypeSampler) {
                type = MTLFunctionTypeVertex;
                index = (uint32_t)arg.index;
                break;
            }
        }
        if( index == -1 )
        {
            for ( MTLArgument* arg in m_pipeline->m_reflection.fragmentArguments ) {
                if ([arg.name isEqualToString: [NSString stringWithCString:_name encoding:NSUTF8StringEncoding]] && arg.type == MTLArgumentTypeSampler) {
                    type = MTLFunctionTypeFragment;
                    index = (uint32_t)arg.index;
                    break;
                }
            }
        }
        if( index == -1 ){
            return slot;
        }
        for ( uint32_t i = 0;i <m_vecSamplers.size(); ++i) {
            if ( m_vecSamplers[i].samplerIndex == index && m_vecSamplers[i].type == type ) {
                slot.mtlIndex = i;
                break;
            }
        }
        return slot;
    }
    void ArgumentMTL::bind() {
        GetContext( context )
        auto encoder = context->getRenderCommandEncoder();
        if( nil == encoder ) {
            assert(false);
            return;
        }
        for( auto & sampler : m_vecSamplers ) {
            if( sampler.type == MTLFunctionTypeVertex ) {
                [encoder setVertexSamplerState: sampler.samplerState atIndex:sampler.samplerIndex];
                [encoder setVertexTexture:sampler.texture atIndex:sampler.samplerIndex];
            }
            else
            {
                [encoder setFragmentSamplerState: sampler.samplerState atIndex:sampler.samplerIndex];
                [encoder setFragmentTexture:sampler.texture atIndex:sampler.samplerIndex];
            }
        }
        for( auto & uniform : m_vecUniforms ) {
            if( uniform.type == MTLFunctionTypeVertex ) {
                [encoder setVertexBuffer: uniform.buffer offset:uniform.offset atIndex:uniform.uniformIndex];
            }
            else
            {
                [encoder setFragmentBuffer: uniform.buffer offset:uniform.offset atIndex:uniform.uniformIndex];
            }
        }
    }
    
    void ArgumentMTL::setSampler(SamplerSlot _slot, const SamplerState& _sampler, const ITexture* _texture) {
        m_vecSamplers[_slot.mtlIndex].texture = ((TextureMTL*)_texture)->texture();
        m_vecSamplers[_slot.mtlIndex].samplerState = GetSamplerObject( _sampler );
    }
    void ArgumentMTL::setUniform(UniformSlot _slot, const void * _data, size_t _size) {
        // dynamic allocation, static alloc ? dynamic alloc ? argument buffer??
        GetContext(context);
        auto & uniformItem = m_vecUniforms[_slot.mtlIndex];
        context->getTUBOAllocator().allocate( uniformItem.size, uniformItem.offset, &uniformItem.buffer );
        uint8_t * ptr = (uint8_t*)uniformItem.buffer.contents;
        ptr += uniformItem.offset;
        memcpy( ptr, _data, _size );
    }
    void ArgumentMTL::release() {
        delete this;
    }
    
}
