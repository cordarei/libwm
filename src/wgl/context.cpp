#include <iostream>
#include <memory>

#include <windows.h>

#include <wm/exception.hpp>
#include <wm/display.hpp>
#include <wm/window.hpp>
#include <wm/pixelformat.hpp>
#include <wm/context.hpp>
#include <wm/surface.hpp>
#include <wm/configuration.hpp>
#include <wm/pixelformat.hpp>

#include <win32/impl/error.hpp>
#include <win32/impl/display_impl.hpp>
#include <win32/impl/window_impl.hpp>
#include <wgl/impl/surface_impl.hpp>
#include <wgl/impl/pixelformat_impl.hpp>
#include <wgl/impl/dummywindow.hpp>

#include <wm/export.hpp>

namespace wm
{
	struct Context::impl_t
	{
		HGLRC hglrc;
	};

	Context::Context(const PixelFormat &format, Context *shared)
		: impl(new impl_t)
		, display_(&format.configuration().display())
	{
		{
			std::auto_ptr<impl_t> impl_guard(impl); // deletes impl object in case of exception

			wgl::DummyWindow dummywin(display().impl->hInstance);
			wgl::DCGetter getter(dummywin.hwnd);

			// TODO: check dummy window pixel format compatibility here

			PIXELFORMATDESCRIPTOR pfd;
			if(!SetPixelFormat(getter.hdc, format.impl->index, &pfd))
				throw wm::Exception("Can't set pixel format: " + wm::win32::getErrorMsg());

			impl->hglrc = wglCreateContext(getter.hdc);
			if(!impl->hglrc)
				throw wm::Exception("Can't create Context" + wm::win32::getErrorMsg());

			impl_guard.release();
		}

		if(shared)
		{
			if(!wglShareLists(shared->impl->hglrc, impl->hglrc))
			{
				DWORD err = GetLastError();
				wglDeleteContext(impl->hglrc);
				throw Exception("Can't share contexts" + win32::getErrorMsg(err));
			}
		}

	}

	Context::~Context()
	{
		try
		{
			if(!wglDeleteContext(impl->hglrc))
				std::cerr << "Can't delete Context: " << win32::getErrorMsg() << std::endl;
		} catch(...)
		{
		}

		delete impl;
	}

	void WM_EXPORT makeCurrent(Context &context, Surface &draw, Surface &read)
	{
		if(&draw != &read)
			throw wm::Exception("Separate draw and read surfaces not supported");

		HWND hwnd = draw.impl->hwnd;
		HGLRC hglrc = context.impl->hglrc;

		wgl::DCGetter getter(hwnd);

		if(!wglMakeCurrent(getter.hdc, hglrc))
			throw Exception("Can't set current context: " + win32::getErrorMsg());
	}
}
