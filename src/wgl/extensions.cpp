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

		if(!getProcAddress("wglChoosePixelFormatARB", extensions.wglChoosePixelFormatARB) ||
			!getProcAddress("wglGetPixelFormatAttribfvARB", extensions.wglGetPixelFormatAttribfvARB) ||
			!getProcAddress("wglGetPixelFormatAttribivARB", extensions.wglGetPixelFormatAttribivARB))
			throw wm::Exception("Can't initialize WGL_ARB_pixel_format");

		extensions.ARB_pixel_format = true;
		return true;
	}
}

namespace wm
{
	namespace wgl
	{
		void Extensions::init(DummyWindow& dummywin, DummyContext &dummyctx)
		{
			wgl::UseContext usectx(dummywin.hwnd, dummyctx.hglrc);

			if(!getProcAddress("wglGetExtensionsStringARB", wglGetExtensionsStringARB))
				return;

			{
				wgl::DCGetter getter(dummywin.hwnd);
				const char *extstring = wglGetExtensionsStringARB(getter.hdc);
				if(!extstring) throw wm::Exception("NULL extension string");
				make_set(extensions, extstring);
			}

			init_WGL_ARB_pixel_format(*this);
		}

		bool Extensions::supported(const std::string &name) const
		{
			return (extensions.find(name) != extensions.end());
		}
	}
}
