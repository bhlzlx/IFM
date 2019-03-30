//
//  ViewMTL.cpp
//  RendererMTL
//
//  Created by kusugawa on 2019/3/23.
//  Copyright © 2019年 kusugawa. All rights reserved.
//

#include "ViewMTL.h"
#include "ContextMTL.h"
#include "QueueMTL.h"

namespace Ks {
    
    IView* ContextMTL::createView(void* _nativeWindow) {
        ViewMTL* view = new ViewMTL();
        view->m_size = { 0, 0 };
        return view;
    }
    
    void ViewMTL::resize(uint32_t _width, uint32_t _height) {
        GetContext( context );
        //auto layer = context->getMetalLayer();
        //[layer setFrame: NSMakeRect( 0, 0, _width, _height)];
        //[layer setDrawableSize: NSMakeSize( _width, _height)];
        m_size = { _width, _height };
    }
    
    bool ViewMTL::beginFrame() {
        GetContext(context);
        bool rst = context->getGraphicsQueue().enterFrame();
        if( !rst )
            return false;
        //
        return true;
    }
    
    void ViewMTL::endFrame() {
        GetContext(context);
        context->getGraphicsQueue().leaveFrame();
    }
    
    Ks::IRenderPass* ViewMTL::getRenderPass() {
        GetContext(context);
        return &context->getMainRenderPass();
    }
    
    Ks::IContext* ViewMTL::getContext() {
        return GetContextMetal();
    }
    
    void ViewMTL::release() {
        delete this;
    }
}
