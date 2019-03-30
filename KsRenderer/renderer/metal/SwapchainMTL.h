//
//  SwapchainMTL.hpp
//  RendererMTL
//
//  Created by kusugawa on 2019/3/28.
//  Copyright © 2019年 kusugawa. All rights reserved.
//

#pragma once
#include "MetalInc.h"

namespace Ks {
    
    class SwapchainMTL {
    private:
        CAMetalLayer* m_metalLayer;
        id<CAMetalDrawable> m_currentDrawable;
        //
        dispatch_semaphore_t m_semaphore;

    public:
        bool initialize( CAMetalLayer* _layer );
        bool prepareFrame();
        id<CAMetalDrawable> getCurrentDrawable() const;
        dispatch_semaphore_t getSemaphore() const;
    };
}


