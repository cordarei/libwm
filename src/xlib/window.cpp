#include <algorithm>
#include <list>
#include <vector>

#include <X11/Xlib.h>

#include <wm/exception.hpp>
#include <wm/display.hpp>
#include <wm/window.hpp>
#include <wm/event.hpp>

#include "impl/display_impl.hpp"
#include "impl/window_impl.hpp"
#include <xlib/impl/eventreader.hpp>

namespace wm
{
	Window::Window(
		Display& display,
		int screen,
		unsigned int width,
		unsigned int height)
		: impl(new impl_t)
		, display_(display)
		, surface_(0)
	{	
		impl->screen = screen;
	
		impl->window = XCreateWindow(
			display.impl->display,
			RootWindow(display.impl->display, screen),
			0, 0,
			width, height,
			0,
			CopyFromParent,
			InputOutput,
			CopyFromParent,
			0,
			0
			);
			
		if(!impl->window)
			throw wm::Exception("Can't create Window");
		
		if(!XSetWMProtocols(
			display.impl->display,
			impl->window,
			&display.impl->wm_delete_window,
			1))
		{
			XDestroyWindow(display.impl->display, impl->window);
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
			XDestroyWindow(display.impl->display, impl->window);
			throw wm::Exception("Can't set X Window manager protocols (WM_DELETE_WINDOW)");			
		}
		
		// Get event mask from input context and listen to specified events
		long xic_event_mask;
		if(XGetICValues(impl->xic, XNFilterEvents, &xic_event_mask, NULL))
		{
			XDestroyIC(impl->xic);
			XDestroyWindow(display.impl->display, impl->window);
			throw Exception("Can't query event mask from X input context");
		}
		
		XSelectInput(
			display.impl->display,
			impl->window,
			EventReader::event_mask | xic_event_mask
			);
			
		// TODO: better error handling, perhaps wait for X 
		display.impl->registry.add(impl->window, this);
	}
	
	Window::~Window()
	{
		display().impl->registry.remove(impl->window);
		
		XDestroyIC(impl->xic);
		XDestroyWindow(display().impl->display, impl->window);
		
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

	void Window::dispatch(bool block)
	{
		if(block)
		{
			boost::scoped_ptr<const Event> event;

			while(!impl->eventq.poll(event))
			{
				display().dispatch(true);
			}
			
			impl->dispatcher.dispatch(*event);
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

