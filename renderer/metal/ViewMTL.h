//
//  ViewMTL.hpp
//  RendererMTL
//
//  Created by kusugawa on 2019/3/23.
//  Copyright © 2019年 kusugawa. All rights reserved.
//

#pragma once
#include "MetalInc.h"

namespace Ks {
    class ViewMTL: IView {
    private:
        CAMetalLayer* m_layer;
        Size<uint32_t> m_size;
    public:
        virtual void resize(uint32_t _width, uint32_t _height) override;
        virtual void beginFrame() override;
        virtual void endFrame() override;
        virtual Ks::IRenderPass *getRenderPass() override;
        virtual Ks::IContext *getContext() override;
        virtual void release() override;
    };
}
