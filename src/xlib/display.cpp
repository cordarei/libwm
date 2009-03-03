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
	}
	
	Display::~Display()
	{
		XCloseDisplay(impl->display);
	}
	
	class Event;
	
	void Display::dispatch(bool block)
	{
		XEvent event;
		
		if(block)
		{
			XMaskEvent(
				impl->display,
				xlib::event_mask,
				&event
				);
				
			wm::Window* window =  impl->registry.get(event.xany.window);
					
			if(window)
			{
				const Event *ptr = xlib::fromXEvent(*window, event);
				if(ptr) window->impl->eventq.push(ptr); // eventq takes ownership of event object in ptr
			}
		} else
		{
			while(XCheckMaskEvent(
				impl->display,
				xlib::event_mask,
				&event
				))
			{
				wm::Window* window =  impl->registry.get(event.xany.window);
					
				if(window)
				{
					const Event *ptr = xlib::fromXEvent(*window, event);
					if(ptr) window->impl->eventq.push(ptr); // eventq takes ownership of event object in ptr
				}
			}			
		}
	}
}

