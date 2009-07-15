#include <X11/Xlib.h>

#include <wm/exception.hpp>
#include <wm/display.hpp>
#include <wm/window.hpp>

#include "impl/display_impl.hpp"
#include "impl/window_impl.hpp"
#include <xlib/impl/eventreader.hpp>

namespace wm
{
	Display::Display(const char *name)
		: impl(new impl_t)
	{
		impl->display = XOpenDisplay(name);
		if(!impl->display) throw Exception("Can't open Display");
		
		impl->wm_delete_window = XInternAtom(impl->display, "WM_DELETE_WINDOW", False);
		
		impl->xim = XOpenIM(impl->display, 0, 0, 0);
		if(!impl->xim)
		{
			XCloseDisplay(impl->display);
			throw Exception("Can't open X input method");
		}
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
			XNextEvent(
				impl->display,
				&event
				);
			
			wm::Window* window =  impl->registry.get(event.xany.window);
			if(window) EventReader::handleXEvent(*window, event);
		}
		
		// TODO: implement a non-blocking version
	}
}

