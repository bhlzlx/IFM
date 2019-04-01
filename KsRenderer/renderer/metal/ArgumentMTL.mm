//
//  ArgumentMTL.cpp
//  KsRenderer
//
//  Created by kusugawa on 2019/4/1.
//  Copyright © 2019年 kusugawa. All rights reserved.
//

#include "ArgumentMTL.h"

namespace Ks {
    
     UniformSlot ArgumentMTL::getUniformSlot(const char * _name) {
         
     }
    
     SamplerSlot ArgumentMTL::getSamplerSlot(const char * _name) {}
     void ArgumentMTL::setSampler(SamplerSlot _slot, const SamplerState& _sampler, const ITexture* _texture) {}
     void ArgumentMTL::setUniform(UniformSlot _slot, const void * _data, size_t _size) {}
     void ArgumentMTL::release() {}
    
}
