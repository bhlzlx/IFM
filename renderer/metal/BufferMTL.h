//
//  BufferMTL.hpp
//  RendererMTL
//
//  Created by kusugawa on 2019/3/24.
//  Copyright © 2019年 kusugawa. All rights reserved.
//
#pragma once

#include "MetalInc.h"

namespace Ks {
    
    class BufferMTL {
    private:
        id<MTLBuffer> m_buffer;
        BufferMTL( const BufferMTL& _buffer );
        BufferMTL& operator = ( const BufferMTL& _buffer );
    public:
        BufferMTL( id<MTLBuffer> _buffer ) {
            m_buffer = _buffer;
        }
        BufferMTL( BufferMTL&& _buffer ) {
            m_buffer = _buffer.m_buffer;
            _buffer.m_buffer = nil;
        }
        id<MTLBuffer> buffer() {
            return m_buffer;
        }
        //
        void setDataCPUAccess( const void * _data, size_t _length, size_t _offset );
        void setDataGPUAccess( const void * _data, size_t _length, size_t _offset );
        //
        static BufferMTL createBuffer( size_t _size, const void * _data );
    };
    
    class SVBO : public IStaticVertexBuffer {
    private:
        BufferMTL m_buffer;
    public:
        SVBO( BufferMTL&& _buffer ) : m_buffer( std::move(_buffer) ) {
        }
    };
    
    class DVBO : public IDynamicVertexBuffer {
    private:
        BufferMTL m_buffer;
    public:
        DVBO( BufferMTL&& _buffer ) : m_buffer( std::move(_buffer) ){
        }
    };
    
    class IBO : public IIndexBuffer {
    private:
        BufferMTL m_buffer;
    public:
        IBO( BufferMTL&& _buffer ) : m_buffer( std::move(_buffer) ) {
        }
    };
    
}
