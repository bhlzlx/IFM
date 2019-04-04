#pragma once
#include <cstdint>
#include "KsRenderer.h"

namespace Ks {
	class ArchieveProtocol;
}

class KsApplication {
public:
	virtual bool initialize(void* _wnd, Ks::ArchieveProtocol* _archieve) = 0;
	virtual void resize(uint32_t _width, uint32_t _height) = 0;
	virtual void release() = 0;
	virtual void tick() = 0;
	virtual const char * title() = 0;
	virtual Ks::DeviceType contextType() = 0;
};

KsApplication * GetApplication();
