#include <X11/Xlib.h>

#include <wm/exception.hpp>
#include <wm/display.hpp>
#include <wm/window.hpp>

#include "impl/display_impl.hpp"
#include "impl/window_impl.hpp"
#include "impl/eventfactory.hpp"

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
			
			bool filter = XFilterEvent(&event, None);
			
			wm::Window* window =  impl->registry.get(event.xany.window);
			
			if(window)
			{
				const Event *ptr = xlib::fromXEvent(*window, event, filter);
				if(ptr) window->impl->eventq.push(ptr); // eventq takes ownership of event object in ptr
			}
		}
		
		// TODO: implement a non-blocking version
	}
}

