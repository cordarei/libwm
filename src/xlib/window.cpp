#include <memory>
#include <algorithm>
#include <list>
#include <vector>

#include <X11/Xlib.h>

#include <wm/exception.hpp>
#include <wm/display.hpp>
#include <wm/window.hpp>
#include <wm/event.hpp>
#include <wm/pixelformat.hpp>

#include <xlib/impl/display_impl.hpp>
#include <xlib/impl/window_impl.hpp>
#include <xlib/impl/eventreader.hpp>

namespace wm
{
	Window::Window(
		Display& display,
		unsigned int width,
		unsigned int height,
		const PixelFormat& format)
		: impl(new impl_t)
		, display_(display)
		, pixelformat_(format)
		, surface_(0)
	{
		std::auto_ptr<impl_t> impl_guard(impl); // deletes impl object in case of exception
		::Display* xdisplay = display.impl->display;
		int screen = display.impl->screen;

		format.set(*this);

		// Create colormap		
		impl->colormap =
			XCreateColormap(
				xdisplay,
				RootWindow(xdisplay, screen),
				impl->visual,
				AllocNone
				);
		if(impl->colormap == None)
			throw wm::Exception("Can't create X Colormap for Surface");

		// Create window
		XSetWindowAttributes attrib;
		attrib.colormap = impl->colormap;
		unsigned long attribmask = CWColormap;

		impl->window = XCreateWindow(
			display.impl->display,
			RootWindow(xdisplay, screen),
			0, 0,
			width, height,
			0,
			impl->depth,
			InputOutput,
			impl->visual,
			attribmask,
			&attrib
			);
			
		if(!impl->window)
		{
			XFreeColormap(xdisplay, impl->colormap);
			throw wm::Exception("Can't create Window");
		}
		
		// Listen for WM_DELETE_WINDOW
		if(!XSetWMProtocols(
			xdisplay,
			impl->window,
			&display.impl->wm_delete_window,
			1))
		{
			XDestroyWindow(xdisplay, impl->window);
			XFreeColormap(xdisplay, impl->colormap);
			throw wm::Exception("Can't set X Window manager protocols (WM_DELETE_WINDOW)");
		}
		
		// Create X input context
		impl->xic = XCreateIC(
			display.impl->xim,
			XNInputStyle, XIMPreeditNothing | XIMStatusNothing,	// we don't have a status or a pre-edit window
			XNClientWindow, impl->window,
			XNFocusWindow, impl->window,
			NULL	// XCreateIC va_list NULL terminator (must be NULL, not 0 to avoid gcc compiler warnings)
			);
		if(!impl->xic)
		{
			XDestroyWindow(xdisplay, impl->window);
			XFreeColormap(xdisplay, impl->colormap);
			throw wm::Exception("Can't set X Window manager protocols (WM_DELETE_WINDOW)");			
		}
		
		// Get event mask from input context and listen to specified events
		long xic_event_mask;
		if(XGetICValues(impl->xic, XNFilterEvents, &xic_event_mask, NULL))
		{
			XDestroyIC(impl->xic);
			XDestroyWindow(xdisplay, impl->window);
			XFreeColormap(xdisplay, impl->colormap);
			throw Exception("Can't query event mask from X input context");
		}
		
		XSelectInput(
			xdisplay,
			impl->window,
			EventReader::event_mask | xic_event_mask
			);
			
		// Add this window to Display's registry
		display.impl->registry.add(impl->window, this);
		
		impl_guard.release();
	}
	
	Window::~Window()
	{
		display().impl->registry.remove(impl->window);
		
		::Display* xdisplay = display().impl->display;		
		
		XDestroyIC(impl->xic);
		XDestroyWindow(xdisplay, impl->window);		
		XFreeColormap(xdisplay, impl->colormap);

		
		delete impl;
	}
	
	void Window::show()
	{
		XMapRaised(display().impl->display, impl->window);
		XFlush(display().impl->display);
		// TODO: Handle errors
	}
	
	void Window::hide()
	{
		XUnmapWindow(display().impl->display, impl->window);
	}
	
	void Window::getSize(unsigned int &width, unsigned int &height)
	{
		::Window root;
		int x, y;
		unsigned int w, h;
		unsigned int border, depth;
	
		if(!XGetGeometry(
			display().impl->display,
			impl->window,
			&root,
			&x, &y,
			&w, &h,
			&border,
			&depth
			))
			throw Exception("Can't get X11 Window geometry");
			
		width = w;
		height = h;
	}
	
	void Window::resize(unsigned int width, unsigned int height)
	{
		XResizeWindow(
			display().impl->display,
			impl->window,
			width,
			height);
	}
	
	void Window::setTitle(const char *title)
	{
		if(!title) return;
	
		Xutf8SetWMProperties(
			display().impl->display,
			impl->window,
			title,	// window_name
			title,	// icon_name
			0, 0,	// argc, argv
			0, 0, 0); // XSizeHints, XWMHints, XClassHint
	}

	void Window::fullscreen(bool full)
	{
		xlib::EWMH &ewmh = display().impl->ewmh;
		
		if(ewmh._net_wm_state_fullscreen)
		{
			ewmh.set_wm_state(impl->window,
				full ? xlib::EWMH::_NET_WM_STATE_ADD : xlib::EWMH::_NET_WM_STATE_REMOVE,
				ewmh._net_wm_state_fullscreen,
				0);
		} else
		{
			throw wm::Exception("Can't set full screen Window: Extended Window Manager Hints not supported");
		}
	}

	void Window::dispatch(bool block)
	{
		if(block)
		{
			boost::scoped_ptr<const Event> event;

			while(!impl->eventq.poll(event))
			{
				display().dispatch(true);
			}
			
			do
			{
				impl->dispatcher.dispatch(*event);
			} while(impl->eventq.poll(event));
		} else
		{
			boost::scoped_ptr<const Event> event;
			while(impl->eventq.poll(event))
			{
				impl->dispatcher.dispatch(*event);
			}
		}
	}
		
	common::Dispatcher& Window::dispatcher()
	{
		return impl->dispatcher;
	}
}

