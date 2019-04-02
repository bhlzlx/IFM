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
        void * m_raw;
        BufferUsageFlagBits m_usage;
        BufferMTL( const BufferMTL& _buffer );
        BufferMTL& operator = ( const BufferMTL& _buffer );
    public:
        BufferMTL() : m_buffer(nil), m_usage( BufferUsageUniform ) {
        }
        BufferMTL( id<MTLBuffer> _buffer ) {
            m_buffer = _buffer;
        }
        BufferMTL( BufferMTL&& _buffer ) {
            m_buffer = _buffer.m_buffer;
            _buffer.m_buffer = nil;
        }
        BufferMTL& operator = ( BufferMTL&& _buffer ) {
            m_buffer = _buffer.m_buffer;
            _buffer.m_buffer = nil;
            return *this;
        }
        id<MTLBuffer> buffer() {
            return m_buffer;
        }
        void release() {
            delete this;
        }
        
        void setDataBlocked( const void * _data, size_t _length, size_t _offset );
        void setDataQueued( const void * _data, size_t _length, size_t _offset );
        //
        static BufferMTL* createBuffer( size_t _size, const void * _data, BufferUsageFlagBits _usage );
        
        ~BufferMTL() {

        }
    };
    
    class SVBOMTL : public IStaticVertexBuffer {
    private:
        BufferMTL m_buffer;
    public:
        SVBOMTL( BufferMTL&& _buffer ) : m_buffer( std::move(_buffer) ) {
        }
        virtual void setData( const void * _data, size_t _size, size_t _offset) override;
        virtual void release() override;
        virtual size_t getSize() override;
        virtual BufferType getType() override;
    };
    
    class DVBOMTL : public IDynamicVertexBuffer {
    private:
        BufferMTL m_buffer;
    public:
        DVBOMTL( BufferMTL&& _buffer ) : m_buffer( std::move(_buffer) ){
        }
        virtual void setData( const void * _data, size_t _size, size_t _offset) override;
        virtual void release() override {
            delete this;
        }
        virtual size_t getSize() override {
            return m_buffer.buffer().length;
        }
        virtual BufferType getType() override {
            return DVBO;
        }
    };
    
    class IBOMTL : public IIndexBuffer {
    private:
        BufferMTL m_buffer;
    public:
        IBOMTL( BufferMTL&& _buffer ) : m_buffer( std::move(_buffer) ) {
        }
        virtual void setData( const void * _data, size_t _size, size_t _offset) override;
        virtual void release() override {
            delete this;
        }
        virtual size_t getSize() override {
            return m_buffer.buffer().length;
        }
        virtual BufferType getType() override {
            return IBO;
        }
    };
    
}
