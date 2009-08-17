#include <memory>

#include <wm/exception.hpp>
#include <wm/display.hpp>
#include <wm/window.hpp>
#include <wm/surface.hpp>
#include <wm/events/resizeevent.hpp>

#include <glx/impl/surface_impl.hpp>
#include <glx/impl/pixelformat_impl.hpp>
#include <glx/impl/configuration_impl.hpp>
#include <xlib/impl/display_impl.hpp>
#include <xlib/impl/window_impl.hpp>

namespace wm
{
	Surface::Surface(Window &window)
		: impl(new impl_t)
		, window_(&window)
	{
		std::auto_ptr<impl_t> impl_guard(impl); // deletes impl object in case of exception
		impl->extensions = &window.pixelformat().configuration().impl->extensions;
		
		const PixelFormat& format = window.pixelformat();

		if(window.surface_)
			throw wm::Exception("Window already has an attached surface");
			
		::Display *xdisplay = window.display().impl->display;
		impl->xdisplay = xdisplay;

#ifdef GLX_VERSION_1_3
		impl->glxwindow = 0;
		if(impl->extensions->supported(1, 3))
		{
			impl->glxwindow =
				impl->extensions->glXCreateWindow(
					xdisplay,
					format.impl->fbconfig,
					window.impl->window,
					0
					);
		}
#endif

		window.surface_ = this;
		
		impl_guard.release();
	}
	
	Surface::~Surface()
	{
		window().surface_ = 0;
		
#ifdef GLX_VERSION_1_3
		::Display* xdisplay = window().display().impl->display;
		if(impl->extensions->supported(1, 3))
			impl->extensions->glXDestroyWindow(xdisplay, impl->glxwindow);
#endif
		delete impl;
	}

	void Surface::swap()
	{
		::Display *xdisplay = window().display().impl->display;

#ifdef GLX_VERSION_1_3
		if(impl->extensions->supported(1, 3))
		{
			glXSwapBuffers(xdisplay, impl->glxwindow);
		} else
#endif
		{
			glXSwapBuffers(xdisplay, window().impl->window);
		}
	}
	
	void Surface::getSize(unsigned int &width, unsigned int &height)
	{
		unsigned int w, h;
		
		::Display *xdisplay = window().display().impl->display;
		const glx::Extensions& extensions = *impl->extensions;
#ifdef GLX_VERSION_1_3
		if(extensions.supported(1, 3))
		{
			extensions.glXQueryDrawable(xdisplay, impl->glxwindow, GLX_WIDTH, &w);
			extensions.glXQueryDrawable(xdisplay, impl->glxwindow, GLX_HEIGHT, &h);
		} else
#endif
		{
			window().getSize(w, h);
		}
		
		width = w;
		height = h;
	}
}

