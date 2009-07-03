#include <windows.h>

#include <wm/window.hpp>
#include <wm/surface.hpp>
#include <wm/pixelformat.hpp>

#include <win32/impl/error.hpp>
#include <win32/impl/window_impl.hpp>
#include <wgl/impl/surface_impl.hpp>
#include <wgl/impl/pixelformat_impl.hpp>

namespace wm
{
	Surface::Surface(Window &window, PixelFormat const &format)
		: impl(new impl_t)
	{
		impl->window = &window;
		HWND hwnd = window.impl->hwnd;

		HDC hdc = GetDC(hwnd);
		if(!hdc)
			throw wm::Exception("Can't get win32 device context" + wm::win32::getErrorMsg());
		
		PIXELFORMATDESCRIPTOR pfd;
		if(!SetPixelFormat(hdc, format.impl->index, &pfd))
		{
			DWORD err = GetLastError();
			ReleaseDC(hwnd, hdc);
			throw wm::Exception("Can't set pixel format" + wm::win32::getErrorMsg(err));
		}

		ReleaseDC(hwnd, hdc);

		window.surface_ = this;
	}

	Surface::~Surface()
	{
		// NOTE: window.surface_ intentionally left non-NULL,
		// because SetPixelFormat can only be done once per window
		delete impl;
	}

	void Surface::swap()
	{
		HWND hwnd = impl->window->impl->hwnd;

		HDC hdc = GetDC(hwnd);
		if(!hdc)
			throw Exception("Can't get handle to win32 device context" + win32::getErrorMsg());

		if(!SwapBuffers(hdc))
		{
			DWORD err = GetLastError();
			ReleaseDC(hwnd, hdc);
			throw Exception("Can't swap buffers: " + win32::getErrorMsg(err));
		}

		ReleaseDC(hwnd, hdc);
	}
}