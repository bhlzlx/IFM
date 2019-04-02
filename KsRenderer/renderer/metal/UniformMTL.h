//
//  UniformMTL.hpp
//  KsRenderer
//
//  Created by kusugawa on 2019/4/2.
//  Copyright © 2019年 kusugawa. All rights reserved.
//

#pragma once
#include "MetalInc.h"
#include "BufferMTL.h"
#include <vector>

namespace Ks {
    
    const static uint32_t UniformPoolSize = 1024 * 1024; // 1MB
    
    class TransientUniformPool {
    private:
        BufferMTL m_buffer;
        uint32_t m_offset;
        bool m_avail;
    public:
        TransientUniformPool() {
        }
        bool initialize();
        bool allocate( size_t _size, uint32_t& offset_, __strong id<MTLBuffer>* buffer_ );
        void reset();
        bool avail();
    };
    
    class TransientUniformAllocator {
    private:
        std::vector<TransientUniformPool> m_vecPools;
        size_t m_current;
    public:
        TransientUniformAllocator() {
        }
        void initialize();
        void allocate( size_t _size, uint32_t& offset_, __strong id<MTLBuffer>* buffer_ );
        void reset();
    };
    
}
