//
//  ShaderModule.hpp
//  KsRenderer
//
//  Created by kusugawa on 2019/3/31.
//  Copyright © 2019年 kusugawa. All rights reserved.
//

#pragma once
#include "MetalInc.h"

namespace Ks {
    class ShaderModuleMTL {
    private:
        id<MTLFunction> m_function;
        MTLFunctionType m_type;
    public:
        ShaderModuleMTL():
            m_shaderModule( nil )
        {
        }
        id<MTLFunction> shader(){
            return m_shaderModule;
        }
        
        // _shaderText : the shader program text content
        // _type : vertex, fragment, compute or other shader types
        static bool createShaderModule( const char * _shaderText, MTLFunctionType _type, ShaderModuleMTL& _sm );
    };
}
