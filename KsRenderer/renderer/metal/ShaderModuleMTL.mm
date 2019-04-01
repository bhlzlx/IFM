//
//  ShaderModule.cpp
//  KsRenderer
//
//  Created by kusugawa on 2019/3/31.
//  Copyright © 2019年 kusugawa. All rights reserved.
//

#include "ShaderModuleMTL.h"
#include "ContextMTL.h"

namespace Ks {
    
    bool ShaderModuleMTL::createShaderModule( const char * _shaderText, MTLFunctionType _type, ShaderModuleMTL& _sm ) {
        GetContext(context)
        id<MTLDevice> device = context->getDevice();
        NSString * source = [NSString stringWithUTF8String:_shaderText];
        NSError* compileError = nil;
        id<MTLLibrary> library = [device newLibraryWithSource:source options:nil error:&compileError];
        if( nil == library ) {
            return false;
        }
        id<MTLFunction> func = [library newFunctionWithName:@"main"];
        if( nil == func ){
            return false;
        }
        _sm.m_function = func;
        _sm.m_type = _type;
        return true;
    }
}
