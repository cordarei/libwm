#include <memory>

#include <X11/Xlib.h>

#include <wm/exception.hpp>
#include <wm/display.hpp>
#include <wm/window.hpp>

#include <xlib/impl/display_impl.hpp>
#include <xlib/impl/window_impl.hpp>
#include <xlib/impl/eventreader.hpp>

namespace wm
{
	Display::Display(const char *name, int screen)
		: impl(new impl_t)
	{
		std::auto_ptr<impl_t> impl_guard(impl); // deletes impl object in case of exception
	
		impl->display = XOpenDisplay(name);
		if(!impl->display) throw Exception("Can't open Display");
		
		impl->screen = (screen == -1) ? DefaultScreen(impl->display) : screen;

		impl->xim = 0;
		try
		{		
			// Get atoms for WM_DELETE_WINDOW
			impl->wm_delete_window = XInternAtom(impl->display, "WM_DELETE_WINDOW", False);
			
			// Initialize Extended Window Manager Hints
			impl->ewmh.init(impl->display, impl->screen);
		
			// Open X Input method
			impl->xim = XOpenIM(impl->display, 0, 0, 0);
			if(!impl->xim)
				throw Exception("Can't open X input method");
		} catch(...)
		{
			XCloseDisplay(impl->display);
			if(impl->xim) XCloseIM(impl->xim);
			
			throw;
		}
		
		impl_guard.release();
	}
	
	Display::~Display()
	{
		XCloseIM(impl->xim);
		
		XCloseDisplay(impl->display);
		
		delete impl;
	}
	
	class Event;
	
	void Display::dispatch(bool block)
	{
		XEvent event;
		
		if(block)
		{
			XNextEvent(impl->display, &event);
			
			wm::Window* window =  impl->registry.get(event.xany.window);
			if(window) EventReader::handleXEvent(*window, event);
		} else
		{
			while(XCheckMaskEvent(impl->display, ~0, &event) ||
				XCheckTypedEvent(impl->display, ClientMessage, &event))
			{
				wm::Window* window =  impl->registry.get(event.xany.window);
				if(window) EventReader::handleXEvent(*window, event);				
			}
		}
	}
}

