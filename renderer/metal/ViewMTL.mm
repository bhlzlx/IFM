//
//  ViewMTL.cpp
//  RendererMTL
//
//  Created by kusugawa on 2019/3/23.
//  Copyright © 2019年 kusugawa. All rights reserved.
//

#include "ViewMTL.h"

namespace Ks {
    void ViewMTL::resize(uint32_t _width, uint32_t _height) {
        [m_layer setFrame: NSMakeRect( 0, 0, _width, _height)];
        [m_layer setDrawableSize: NSMakeSize( _width, _height)];
        m_size = { _width, _height };
    }
    
    void ViewMTL::beginFrame() {
        
    }
    
    void ViewMTL::endFrame() {
        
    }
    
    Ks::IRenderPass* ViewMTL::getRenderPass() {
        return nullptr;
    }
    
    Ks::IContext* ViewMTL::getContext() {
        return GetContextMetal();
    }
    
    void ViewMTL::release() {
        delete this;
    }
}
