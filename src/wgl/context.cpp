#include <iostream>

#include <windows.h>

#include <wm/exception.hpp>
#include <wm/pixelformat.hpp>
#include <wm/context.hpp>
#include <wm/window.hpp>
#include <wm/surface.hpp>
#include <wm/configuration.hpp>
#include <wm/pixelformat.hpp>

#include <win32/impl/error.hpp>
#include <win32/impl/window_impl.hpp>
#include <wgl/impl/surface_impl.hpp>

#include <wm/export.hpp>

namespace wm
{
	struct Context::impl_t
	{
		HGLRC hglrc;
	};

	Context::Context(Window& window, const PixelFormat &format, Context *shared)
		: impl(new impl_t)
		, display_(format.configuration->display())
	{
		HWND hwnd = window.impl->hwnd;

		HDC hdc = GetDC(hwnd);
		if(!hdc)
			throw wm::Exception("Can't get win32 device context" + wm::win32::getErrorMsg());

		impl->hglrc = wglCreateContext(hdc);
		if(!impl->hglrc)
		{
			DWORD err = GetLastError();
			ReleaseDC(hwnd, hdc);
			throw wm::Exception("Can't create Context" + wm::win32::getErrorMsg(err));
		}

		ReleaseDC(hwnd, hdc);

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

		HWND hwnd = draw.impl->window->impl->hwnd;
		HGLRC hglrc = context.impl->hglrc;

		HDC hdc = GetDC(hwnd);
		if(!hdc)
			throw wm::Exception("Can't get win32 device context" + win32::getErrorMsg());

		if(!wglMakeCurrent(hdc, hglrc))
		{
			DWORD err = GetLastError();
			ReleaseDC(hwnd, hdc);	
			throw Exception("Can't set current context: " + win32::getErrorMsg(err));
		}

		ReleaseDC(hwnd, hdc);	
	}
}
