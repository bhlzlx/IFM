#include <KsRenderer.h>
#include "MetalInc.h"

namespace Ks {
    
    class TextureMTL : public ITexture {
    private:
        TextureDescription m_description;
        id<MTLTexture> m_texture;
    public:
        virtual const Ks::TextureDescription &getDesc() const override {
            return m_description;
        }
        //
        virtual void setSubData( const void * _data, size_t _length, const TextureRegion& _region ) override;
        //
        virtual void setSubData(const void * _data, size_t _length, const TextureRegion& _region, uint32_t _mipCount ) override;
        //
        virtual void release() override;
        
        id<MTLTexture> texture() {
            return m_texture;
        }
        //
        static TextureMTL* createTexture( const TextureDescription& _desc, id<MTLTexture> _texture );
    };
    
}
