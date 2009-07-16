#include <windows.h>

#include <wm/window.hpp>
#include <wm/surface.hpp>
#include <wm/pixelformat.hpp>

#include <win32/impl/error.hpp>
#include <win32/impl/window_impl.hpp>
#include <wgl/impl/surface_impl.hpp>
#include <wgl/impl/pixelformat_impl.hpp>
#include <wgl/impl/dummywindow.hpp>

namespace wm
{
	Surface::Surface(Window &window)
		: impl(new impl_t)
	{
		impl->window = &window;
		HWND hwnd = window.impl->hwnd;
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

		wgl::DCGetter getter(hwnd);

		if(!SwapBuffers(getter.hdc))
			throw Exception("Can't swap buffers: " + win32::getErrorMsg());
	}
}