#pragma once
#include <cstdint>
#include <KsRenderer.h>

#ifdef _WIN32
#include <Windows.h>
#else
#endif

namespace Ks {
	class ArchieveProtocol;

	inline IContext* LoadContext(DeviceType _type) {

		typedef Ks::IContext* (*__cdecl GetContextFunction)();
		GetContextFunction GetContext = nullptr;

		if (_type == GLES3) {
#ifdef _WIN32
			HMODULE dll = NULL;
			dll = ::LoadLibraryA("RendererES3d.dll");
			if (!dll) {
				dll = ::LoadLibraryA("RendererES3.dll");
			}
			GetContext = (GetContextFunction)::GetProcAddress(dll, "GetContextES3");
#else
			void* ES3RD = dlopen("libRendererES3.so", RTLD_NOW | RTLD_LOCAL);
			if (!ES3RD) return false;
			GetContext = reinterpret_cast<GetContextFunction>(dlsym(ES3RD, "GetContextES3"));
#endif
		}
		else if (_type == Vulkan) {
			// vulkan
#ifdef _WIN32
			HMODULE dll = NULL;
			dll = ::LoadLibraryA("RendererVkd.dll");
			if (!dll) {
				dll = ::LoadLibraryA("RendererVk.dll");
			}
			GetContext = (GetContextFunction)::GetProcAddress(dll, "GetContextVulkan");
#else
			void* VKRD = dlopen("libRendererVk.so", RTLD_NOW | RTLD_LOCAL);
			if (!VKRD) return false;
			GetContext = reinterpret_cast<GetContextFunction>(dlsym(VKRD, "GetContextVulkan"));
#endif
		}
		else if (_type == OpenGLCore) {
			// opengl core
		}
		else if (_type == Software) {
			// swift shader...
		}
		return GetContext();
	}

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