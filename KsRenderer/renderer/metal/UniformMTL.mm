//
//  UniformMTL.cpp
//  KsRenderer
//
//  Created by kusugawa on 2019/4/2.
//  Copyright © 2019年 kusugawa. All rights reserved.
//

#include "UniformMTL.h"
#include "ContextMTL.h"


namespace Ks {
    
    bool TransientUniformPool::initialize() {
        BufferMTL* buffer = BufferMTL::createBuffer( UniformPoolSize, nullptr, BufferUsageUniform );
        m_buffer = std::move(*buffer);
        delete buffer;
        m_offset = 0;
        return true;
    }
    
    bool TransientUniformPool::allocate( size_t _size, uint32_t& offset_, __strong id<MTLBuffer>* buffer_ ) {
        if( m_buffer.buffer().length - m_offset > _size ) {
            offset_ = m_offset;
            *buffer_ = m_buffer.buffer();
            m_offset += _size;
            m_offset = ( m_offset + 63 ) & ~(63);
            return true;
        }
        return false;
    }
    
    void TransientUniformPool::reset() {
        m_offset = 0;
    }
    
    void TransientUniformAllocator::allocate( size_t _size, uint32_t& offset_, __strong id<MTLBuffer>* buffer_ ) {
        if( !m_vecPools[m_current].allocate( _size, offset_, buffer_ ) ){
            if( m_vecPools.size() == m_current ) {
                m_vecPools.resize(m_current + 1);
                m_vecPools.back().initialize();
            }
            bool rst = m_vecPools[m_current].allocate( _size, offset_, buffer_ );
            if( !rst ) {
                assert(false);
            }
        }
    }
    
    void TransientUniformAllocator::reset() {
        m_current = 0;
        for( auto& pool : m_vecPools ) {
            pool.reset();
        }
    }
    
    void TransientUniformAllocator::initialize() {
        m_vecPools.reserve(16);
        m_vecPools.resize(1);
        m_vecPools.back().initialize();
    }
}
