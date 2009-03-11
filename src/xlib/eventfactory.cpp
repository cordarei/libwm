#include <iostream>
#include <map>

#include <X11/Xlib.h>

#include <wm/event.hpp>

namespace 
{
	const char *event_names[] = {
	   "",
	   "",
	   "KeyPress",
	   "KeyRelease",
	   "ButtonPress",
	   "ButtonRelease",
	   "MotionNotify",
	   "EnterNotify",
	   "LeaveNotify",
	   "FocusIn",
	   "FocusOut",
	   "KeymapNotify",
	   "Expose",
	   "GraphicsExpose",
	   "NoExpose",
	   "VisibilityNotify",
	   "CreateNotify",
	   "DestroyNotify",
	   "UnmapNotify",
	   "MapNotify",
	   "MapRequest",
	   "ReparentNotify",
	   "ConfigureNotify",
	   "ConfigureRequest",
	   "GravityNotify",
	   "ResizeRequest",
	   "CirculateNotify",
	   "CirculateRequest",
	   "PropertyNotify",
	   "SelectionClear",
	   "SelectionRequest",
	   "SelectionNotify",
	   "ColormapNotify",
	   "ClientMessage",
	   "MappingNotify"
		};

	const wm::Event* makeExpose(
		wm::Window& window,
		const XEvent &event,
		bool)
	{
		return new wm::ExposeEvent(
			window,
			event.xexpose.x,
			event.xexpose.y,
			event.xexpose.width,
			event.xexpose.height);
	}


	const wm::Event* makeButton(
		wm::Window& window,
		const XEvent &event,
		bool)
	{
		return new wm::ButtonEvent(
			window,
			event.xbutton.x,
			event.xbutton.y,
			event.xbutton.button,
			event.type == ButtonPress);
	}
	
	const wm::Event* makeFocus(
		wm::Window& window,
		const XEvent &event,
		bool)
	{
		return new wm::FocusEvent(
			window,
			event.type == FocusIn);
	}
	
	const wm::Event* makeMouseOver(
		wm::Window& window,
		const XEvent &event,
		bool)
	{
		return new wm::MouseOverEvent(
			window,
			event.xcrossing.x,
			event.xcrossing.y,
			event.type == EnterNotify);
	}
		
	const wm::Event* makeShow(
		wm::Window& window,
		const XEvent &event,
		bool)
	{
		return new wm::ShowEvent(
			window,
			event.type == MapNotify
			);
	}
	
}

#include <wm/display.hpp>
#include <wm/window.hpp>
#include "impl/window_impl.hpp"
#include "impl/display_impl.hpp"
#include "impl/eventfactory.hpp"

namespace wm
{
	class EventReader
	{
		public:
			static const Event* makeClient(
				wm::Window &window,
				const XEvent &event,
				bool)
			{
				if(event.xclient.data.l[0] ==
					window.display().impl->wm_delete_window)
				{
					return new wm::CloseEvent(window);
				}
				
				return 0;
			}
			
			static const wm::Event* makeResize(
				wm::Window& window,
				const XEvent &event,
				bool)
			{
				if(event.xconfigure.width == window.impl->width &&
					event.xconfigure.height == window.impl->height)
					return 0;

				window.impl->width = event.xconfigure.width;	
				window.impl->height = event.xconfigure.height;	

				return new wm::ResizeEvent(
					window,
					event.xconfigure.width,
					event.xconfigure.height
					);
			}
			
			static const wm::Event* makeKey(
				wm::Window& window,
				const XEvent &event,
				bool filter)
			{
				if(event.type == KeyPress && !filter)
				{
					char utf[6] = { 0, 0, 0, 0, 0, 0 };
					KeySym keysym;
					Status status;
					
					// XmbLookupString output encoding depends on current locale
					// XwcLookupString also uses locale and nonportable wchar_t
					// Xutf8LookupString always returns UTF-8 and we always want unicode, so use it
					int len = Xutf8LookupString(
						window.impl->xic,
						const_cast<XKeyEvent*>(&event.xkey), // Xlib is not const correct
						utf,
						sizeof(utf)-1,
						&keysym,
						&status
						);
					utf[len] = 0;	// add null terminator
					
					std::cout << "XEventFilter: " << (filter ? "true" : "false") << std::endl;
					std::cout << "Xutf8LookupString returned: " << len << std::endl;
						
					switch(status)
					{
					case XBufferOverflow:
						std::cout << "buffer overflow" << std::endl;
						break;
					case XLookupNone:
						std::cout << "buffer lookup none" << std::endl;
						break;
					case XLookupKeySym:
					case XLookupBoth:	
						std::cout << "Lookup keysym" << std::endl;
						if(status == XLookupKeySym) break;
					case XLookupChars:
						std::cout << "Lookup chars: " << utf << std::endl;
						break;
					}
				}
				
				if(filter)
				{
					std::cout << "XEventFilter: true" << std::endl;	
				}
			
				return new wm::KeyEvent(
					window,
					event.type == KeyPress);
			}


	};

	namespace xlib
	{
		long event_mask = 
			StructureNotifyMask |
			ExposureMask |
//			PointerMotionMask |
			ButtonPressMask |
			ButtonReleaseMask |
			KeyPressMask |
			KeyReleaseMask |
			EnterWindowMask |
			LeaveWindowMask |
			FocusChangeMask
			;

	
		const Event* fromXEvent(
			wm::Window& window,
			const XEvent &event,
			bool filter
			)
		{
			typedef const Event* (DispatchFunc)(
				wm::Window& window,
				const XEvent &,
				bool);

			static const struct Registry
			{
				Registry()
				{
					map[Expose] = makeExpose;
					map[ButtonPress] = makeButton;
					map[ButtonRelease] = makeButton;
					map[FocusIn] = makeFocus;
					map[FocusOut] = makeFocus;
					map[EnterNotify] = makeMouseOver;
					map[LeaveNotify] = makeMouseOver;
					map[MapNotify] = makeShow;
					map[UnmapNotify] = makeShow;
					
					map[KeyPress] = &EventReader::makeKey;
					map[KeyRelease] = &EventReader::makeKey;
					map[ConfigureNotify] = &EventReader::makeResize;
					map[ClientMessage] = &EventReader::makeClient;
				}
			
				typedef std::map<int, DispatchFunc*> map_t;
				map_t map;
			} registry;

			Registry::map_t::const_iterator iter =
				registry.map.find(event.type);
			if(iter != registry.map.end())
			{
				DispatchFunc *func = iter->second;
				return func(window, event, filter);
			} else
			{
				std::cout << "Unhandled event " << event_names[event.type] << std::endl;
				return 0;
			}
		}
	}
}

