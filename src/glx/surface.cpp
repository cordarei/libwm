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
	void Surface::impl_t::ResizeHandler::handle(const ResizeEvent& event)
	{
		Window &window = event.window();
		Surface &surface = window.surface();

		XResizeWindow(
			surface.impl->xdisplay,
			surface.impl->subwindow,
			event.width(),
			event.height());

	}

	Surface::Surface(Window &window, const PixelFormat& format)
		: impl(new impl_t(window))
	{
		if(window.surface_)
			throw wm::Exception("Window already has an attached surface");
			
		::Display *xdisplay = window.display().impl->display;
		impl->xdisplay = xdisplay;

		unsigned int width, height;
		window.getSize(width, height);

		XVisualInfo* visualinfo = 
			glXGetVisualFromFBConfig(
				xdisplay,
				format.impl->fbconfig
				);
				
		if(!visualinfo)
			throw wm::Exception("Can't get X Visual info from framebuffer configuration");
			
		impl->colormap =
			XCreateColormap(
				xdisplay,
				RootWindow(xdisplay, window.impl->screen),
				visualinfo->visual,
				AllocNone
				);
			
		if(impl->colormap == None)
		{
			XFree(visualinfo);
			throw wm::Exception("Can't create X Colormap for Surface");
		}
		
		XSetWindowAttributes attrib;
		attrib.colormap = impl->colormap;
		
		impl->subwindow = 
			XCreateWindow(
				xdisplay,
				window.impl->window,
				0, 0,
				width, height,
				0,
				visualinfo->depth,
				InputOutput,
				visualinfo->visual,
				CWColormap,
				&attrib
				);
				
		XFree(visualinfo);

		long event_mask = ExposureMask;
		XSelectInput(
			xdisplay,
			impl->subwindow,
			event_mask
			);
				
		XMapWindow(
			xdisplay,
			impl->subwindow
			);
			
		impl->glxwindow =
			glXCreateWindow(
				xdisplay,
				format.impl->fbconfig,
				impl->subwindow,
				0
				);
		
		window.surface_ = this;
		window.display().impl->registry.add(impl->subwindow, &window);
		impl->connection.connect();
	}
	
	Surface::~Surface()
	{
		impl->connection.disconnect();
		impl->window->display().impl->registry.remove(impl->subwindow);
		impl->window->surface_ = 0;

		::Display* xdisplay = impl->window->display().impl->display;
		
		glXDestroyWindow(
			xdisplay,
			impl->glxwindow);
			
		XDestroyWindow(
			xdisplay,
			impl->subwindow);		

		XFreeColormap(
			xdisplay,
			impl->colormap);
	
		delete impl;
	}
	
	void Surface::swap()
	{
		::Display *xdisplay = impl->window->display().impl->display;
		glXSwapBuffers(xdisplay, impl->glxwindow);
	}
}

