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
		const XEvent &event
		)
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
		const XEvent &event
		)
	{
		return new wm::ButtonEvent(
			window,
			event.xbutton.x,
			event.xbutton.y,
			event.xbutton.button,
			event.type == ButtonPress);
	}
	
	const wm::Event* makeKey(
		wm::Window& window,
		const XEvent &event
		)
	{
		return new wm::KeyEvent(
			window,
			event.type == KeyPress);
	}
	
	const wm::Event* makeFocus(
		wm::Window& window,
		const XEvent &event)
	{
		return new wm::FocusEvent(
			window,
			event.type == FocusIn);
	}
	
	const wm::Event* makeMouseOver(
		wm::Window& window,
		const XEvent &event)
	{
		return new wm::MouseOverEvent(
			window,
			event.xcrossing.x,
			event.xcrossing.y,
			event.type == EnterNotify);
	}
	
	const wm::Event* makeResize(
		wm::Window& window,
		const XEvent &event)
	{
		return new wm::ResizeEvent(
			window,
			event.xresizerequest.width,
			event.xresizerequest.height
			);
	}
	
	const wm::Event* makeShow(
		wm::Window& window,
		const XEvent &event)
	{
		return new wm::ShowEvent(
			window,
			event.type == MapNotify
			);
	}
	
}

#include <wm/display.hpp>
#include <wm/window.hpp>
#include "impl/display_impl.hpp"
#include "impl/eventfactory.hpp"

namespace wm
{
	class EventReader
	{
		public:
			static const Event* makeClient(
				wm::Window &window,
				const XEvent &event)
			{
				if(event.xclient.data.l[0] ==
					window.display().impl->wm_delete_window)
				{
					return new wm::CloseEvent(window);
				}
				
				return 0;
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
			FocusChangeMask |
			ResizeRedirectMask
			;

	
		const Event* fromXEvent(
			wm::Window& window,
			const XEvent &event
			)
		{
			typedef const Event* (DispatchFunc)(
				wm::Window& window,
				const XEvent &);

			static const struct Registry
			{
				Registry()
				{
					map[Expose] = makeExpose;
					map[ButtonPress] = makeButton;
					map[ButtonRelease] = makeButton;
					map[KeyPress] = makeKey;
					map[KeyRelease] = makeKey;
					map[FocusIn] = makeFocus;
					map[FocusOut] = makeFocus;
					map[EnterNotify] = makeMouseOver;
					map[LeaveNotify] = makeMouseOver;
					map[ResizeRequest] = makeResize;
					map[MapNotify] = makeShow;
					map[UnmapNotify] = makeShow;
					
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
				return func(window, event);
			} else
			{
				std::cout << "Unhandled event " << event_names[event.type] << std::endl;
				return 0;
			}
		}
	}
}

