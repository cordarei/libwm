#include <wm/exception.hpp>
#include <wm/display.hpp>
#include <wm/window.hpp>
#include <wm/surface.hpp>
#include <wm/events/resizeevent.hpp>

#include <glx/impl/surface_impl.hpp>
#include <glx/impl/pixelformat_impl.hpp>
#include <xlib/impl/display_impl.hpp>
#include <xlib/impl/window_impl.hpp>

namespace wm
{
	Surface::Surface(Window &window)
		: impl(new impl_t(window))
	{
		const PixelFormat& format = window.pixelformat();
	
		if(window.surface_)
			throw wm::Exception("Window already has an attached surface");
			
		::Display *xdisplay = window.display().impl->display;
		impl->xdisplay = xdisplay;
			
		impl->glxwindow =
			glXCreateWindow(
				xdisplay,
				format.impl->fbconfig,
				window.impl->window,
				0
				);
		
		window.surface_ = this;
	}
	
	Surface::~Surface()
	{
		impl->window->surface_ = 0;

		::Display* xdisplay = impl->window->display().impl->display;
		
		glXDestroyWindow(
			xdisplay,
			impl->glxwindow);
	
		delete impl;
	}
	
	void Surface::swap()
	{
		::Display *xdisplay = impl->window->display().impl->display;
		glXSwapBuffers(xdisplay, impl->glxwindow);
	}
}

