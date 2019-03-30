//
//  SwapchainMTL.cpp
//  RendererMTL
//
//  Created by kusugawa on 2019/3/28.
//  Copyright © 2019年 kusugawa. All rights reserved.
//

#include "SwapchainMTL.h"


namespace Ks {

    bool SwapchainMTL::initialize( CAMetalLayer* _layer ) {
        m_semaphore = dispatch_semaphore_create( MaxFlightCount );
        m_metalLayer = _layer;
        return true;
    }
    
    bool SwapchainMTL::prepareFrame() {
        dispatch_semaphore_wait( m_semaphore, -1 );
        m_currentDrawable = m_metalLayer.nextDrawable;
        if( m_currentDrawable == nil ) {
            dispatch_semaphore_signal( m_semaphore );
            return false;
        }
        return true;
    }
    
    id<CAMetalDrawable> SwapchainMTL::getCurrentDrawable() const {
        return m_currentDrawable;
    }
    
    dispatch_semaphore_t SwapchainMTL::getSemaphore() const{
        return m_semaphore;
    }
}
