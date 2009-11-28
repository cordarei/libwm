#include <sstream>
#include <iterator>

#include <windows.h>

#include <wm/exception.hpp>

#include <win32/impl/error.hpp>
#include <wgl/impl/extensions.hpp>
#include <wgl/impl/dummywindow.hpp>

namespace
{
	template <typename T>
	void getProcAddress(const char *name, T *& funcptr)
	{
		PROC ptr = wglGetProcAddress(name);
		if(!ptr)
			throw wm::Exception("Can't load extension function " + (name + (": " + wm::win32::getErrorMsg())));

		funcptr = (T*)(ptr); //*reinterpret_cast<T**>(&ptr);
	}

	void make_set(std::set<std::string> &set, std::string const &str)
	{
		std::istringstream iss(str);
		set.clear();
		set.insert(
			std::istream_iterator<std::string>(iss),
			std::istream_iterator<std::string>()
			);
	}

	bool init_WGL_ARB_pixel_format(wm::wgl::Extensions& extensions)
	{
		if(!extensions.supported("WGL_ARB_pixel_format"))
			return false;

		getProcAddress("wglGetPixelFormatAttribfvARB", extensions.wglGetPixelFormatAttribfvARB);
		getProcAddress("wglGetPixelFormatAttribivARB", extensions.wglGetPixelFormatAttribivARB);

		extensions.ARB_pixel_format = true;
		return true;
	}

	bool init_WGL_ARB_create_context(wm::wgl::Extensions& extensions)
	{
		if(!extensions.supported("WGL_ARB_create_context"))
			return false;

		getProcAddress("wglCreateContextAttribsARB", extensions.wglCreateContextAttribsARB);

		extensions.ARB_create_context = true;
		extensions.ARB_create_context_profile = extensions.supported("WGL_ARB_create_context_profile");

		return true;
	}

	bool init_WGL_ARB_multisample(wm::wgl::Extensions& extensions)
	{
		return (extensions.ARB_multisample = extensions.supported("WGL_ARB_multisample"));
	}

	bool init_WGL_ARB_make_current_read(wm::wgl::Extensions &extensions)
	{
		if(!extensions.supported("WGL_ARB_make_current_read"))
			return false;

		getProcAddress("wglMakeContextCurrentARB", extensions.wglMakeContextCurrentARB);
		getProcAddress("wglGetCurrentReadDCARB", extensions.wglGetCurrentReadDCARB);

		extensions.ARB_make_current_read = true;
		return true;
	}

	bool init_WGL_ARB_framebuffer_sRGB(wm::wgl::Extensions& extensions)
	{
		return (extensions.ARB_framebuffer_sRGB = extensions.supported("WGL_ARB_framebuffer_sRGB"));
	}

	bool init_WGL_EXT_framebuffer_sRGB(wm::wgl::Extensions& extensions)
	{
		return (extensions.EXT_framebuffer_sRGB = extensions.supported("WGL_EXT_framebuffer_sRGB"));
	}

	bool init_WGL_ARB_pixel_format_float(wm::wgl::Extensions& extensions)
	{
		return (extensions.ARB_pixel_format_float = extensions.supported("WGL_ARB_pixel_format_float"));
	}

	bool init_WGL_EXT_pixel_format_packed_float(wm::wgl::Extensions& extensions)
	{
		return (extensions.EXT_pixel_format_packed_float = extensions.supported("WGL_EXT_pixel_format_packed_float"));
	}

	bool init_WGL_EXT_swap_control(wm::wgl::Extensions& extensions)
	{
		if((extensions.EXT_swap_control = extensions.supported("WGL_EXT_swap_control")))
		{
			getProcAddress("wglSwapIntervalEXT", extensions.wglSwapIntervalEXT);
			getProcAddress("wglGetSwapIntervalEXT", extensions.wglGetSwapIntervalEXT);
		}

		return extensions.EXT_swap_control;
	}
}

namespace wm
{
	namespace wgl
	{
		void Extensions::init(DummyWindow& dummywin, DummyContext &dummyctx)
		{
			wgl::DCGetter getter(dummywin.hwnd);
			wgl::UseContext usectx(getter.hdc, dummyctx.hglrc);

			try
			{
				getProcAddress("wglGetExtensionsStringARB", wglGetExtensionsStringARB);
			} catch(wm::Exception&)
			{
				return;
			}

			{
				const char *extstring = wglGetExtensionsStringARB(getter.hdc);
				if(!extstring) throw wm::Exception("NULL extension string");
				make_set(extensions, extstring);
			}

			init_WGL_ARB_pixel_format(*this);
			init_WGL_ARB_create_context(*this);
			init_WGL_ARB_multisample(*this);
			init_WGL_ARB_make_current_read(*this);
			init_WGL_ARB_framebuffer_sRGB(*this);
			init_WGL_EXT_framebuffer_sRGB(*this);
			init_WGL_ARB_pixel_format_float(*this);
			init_WGL_EXT_pixel_format_packed_float(*this);
			init_WGL_EXT_swap_control(*this);
		}

		bool Extensions::supported(const std::string &name) const
		{
			return (extensions.find(name) != extensions.end());
		}
	}
}
