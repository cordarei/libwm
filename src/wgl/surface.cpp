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
		, window_(&window)
	{
		// NOTE: does not throw exceptions, handle impl object cleanup
		// if exceptions are thrown in this constructor 
		impl->hwnd = window.impl->hwnd;
		window.surface_ = this;
	}

	Surface::~Surface()
	{
		window().surface_ = 0;
		delete impl;
	}

	void Surface::swap()
	{
		wgl::DCGetter getter(impl->hwnd);

		if(!SwapBuffers(getter.hdc))
			throw Exception("Can't swap buffers: " + win32::getErrorMsg());
	}
}