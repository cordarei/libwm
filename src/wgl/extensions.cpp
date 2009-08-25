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
	bool getProcAddress(const char *name, T *& funcptr)
	{
		PROC ptr = wglGetProcAddress(name);
		if(!ptr)
		{
			funcptr = 0;
			return false;
		}

		funcptr = (T*)(ptr); //*reinterpret_cast<T**>(&ptr);
		return true;
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

		if(!getProcAddress("wglGetPixelFormatAttribfvARB", extensions.wglGetPixelFormatAttribfvARB) ||
			!getProcAddress("wglGetPixelFormatAttribivARB", extensions.wglGetPixelFormatAttribivARB))
			throw wm::Exception("Can't initialize WGL_ARB_pixel_format");

		extensions.ARB_pixel_format = true;
		return true;
	}

	bool init_WGL_ARB_create_context(wm::wgl::Extensions& extensions)
	{
		if(!extensions.supported("WGL_ARB_create_context"))
			return false;

		if(!getProcAddress("wglCreateContextAttribsARB", extensions.wglCreateContextAttribsARB))
			throw wm::Exception("Can't initialize WGL_ARB_create_context");

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

		if(!getProcAddress("wglMakeContextCurrentARB", extensions.wglMakeContextCurrentARB) ||
			!getProcAddress("wglGetCurrentReadDCARB", extensions.wglGetCurrentReadDCARB))
			throw wm::Exception("Can't initialize WGL_ARB_make_current_read");

		extensions.ARB_make_current_read = true;
		return true;
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

			if(!getProcAddress("wglGetExtensionsStringARB", wglGetExtensionsStringARB))
				return;

			{
				const char *extstring = wglGetExtensionsStringARB(getter.hdc);
				if(!extstring) throw wm::Exception("NULL extension string");
				make_set(extensions, extstring);
			}

			init_WGL_ARB_pixel_format(*this);
			init_WGL_ARB_create_context(*this);
			init_WGL_ARB_multisample(*this);
			init_WGL_ARB_make_current_read(*this);
		}

		bool Extensions::supported(const std::string &name) const
		{
			return (extensions.find(name) != extensions.end());
		}
	}
}
