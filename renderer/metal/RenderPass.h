#pragma once
#include "MetalInc.h"

namespace Ks
{
    class TextureMTL : public ITexture {
    private:
        TextureDescription m_description;
        id<MTLTexture> m_texture;
    public:
        const Ks::TextureDescription &getDesc() const override;
        
        void setSubData(const void *_data, size_t _length, const Ks::ImageRegion &_region) override;
        
        void release() override;
        
    };
}
