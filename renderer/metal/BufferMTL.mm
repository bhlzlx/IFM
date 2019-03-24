//
//  BufferMTL.cpp
//  RendererMTL
//
//  Created by kusugawa on 2019/3/24.
//  Copyright © 2019年 kusugawa. All rights reserved.
//

#include "BufferMTL.h"
#include "ContextMTL.h"

namespace Ks {
    
    BufferMTL BufferMTL::createBuffer( size_t _size, const void * _data ) {
        GetContext(context);
        id<MTLBuffer> buffer = [context->getDevice() newBufferWithBytes:_data length:_size options:MTLResourceOptionCPUCacheModeDefault];
        BufferMTL b(buffer);
        return b;
    }
    
}
