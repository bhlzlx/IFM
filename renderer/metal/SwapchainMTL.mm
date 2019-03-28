//
//  SwapchainMTL.cpp
//  RendererMTL
//
//  Created by kusugawa on 2019/3/28.
//  Copyright © 2019年 kusugawa. All rights reserved.
//

#include "SwapchainMTL.h"


namespace Ks {
    
    class SwapchainMTL {
    private:
        CAMetalLayer* m_metalLayer;
        id<MTLDrawable> m_currentDrawable;
        //
        dispatch_semaphore_t m_semaphore;
    public:
        bool initialize() {
            m_semaphore = dispatch_semaphore_create( MaxFlightCount );
            return true;
        }
        
        bool prepareFrame() {
            dispatch_semaphore_wait( m_semaphore, -1 );
            m_currentDrawable = m_metalLayer.nextDrawable;
            if( m_currentDrawable == nil ){
                dispatch_semaphore_signal( m_semaphore );
                return false;
            }
            return true;
        }
        
        
    };
    
}
