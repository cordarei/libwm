#include <iostream>
#include <list>
#include <map>

#include <boost/ref.hpp>
#include <boost/bind.hpp>

#include <X11/Xlib.h>

#include <wm/eventhandler.hpp>
#include <wm/event.hpp>

#include <common/dispatcher.hpp>

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


	void dispatchExpose(
		wm::Window& window,
		wm::common::Dispatcher& dispatcher,
		const XEvent &event
		)
	{
		wm::ExposeEvent expose(
			window,
			event.xexpose.x,
			event.xexpose.y,
			event.xexpose.width,
			event.xexpose.height);
		dispatcher.dispatch(expose);
	}


	void dispatchButton(
		wm::Window& window,
		wm::common::Dispatcher& dispatcher,
		const XEvent &event
		)
	{
		wm::ButtonEvent button(
			window,
			event.xbutton.x,
			event.xbutton.y,
			event.xbutton.button,
			event.type == ButtonPress);
		dispatcher.dispatch(button);
	}
	
	void dispatchKey(
		wm::Window& window,
		wm::common::Dispatcher& dispatcher,
		const XEvent &event
		)
	{
		wm::KeyEvent key(
			window,
			event.type == KeyPress);
		dispatcher.dispatch(key);
	}
	
	void dispatchFocus(
		wm::Window& window,
		wm::common::Dispatcher& dispatcher,
		const XEvent &event)
	{
		wm::FocusEvent focus(
			window,
			event.type == FocusIn);
		dispatcher.dispatch(focus);
	}
	
	void dispatchMouseOver(
		wm::Window& window,
		wm::common::Dispatcher& dispatcher,
		const XEvent &event)
	{
		wm::MouseOverEvent mouseOver(
			window,
			event.xcrossing.x,
			event.xcrossing.y,
			event.type == EnterNotify);
		dispatcher.dispatch(mouseOver);
	}
	
	void dispatchResize(
		wm::Window& window,
		wm::common::Dispatcher& dispatcher,
		const XEvent &event)
	{
		wm::ResizeEvent resize(
			window,
			event.xresizerequest.width,
			event.xresizerequest.height
			);
		dispatcher.dispatch(resize);
	}
	
	void dispatchShow(
		wm::Window& window,
		wm::common::Dispatcher& dispatcher,
		const XEvent &event)
	{
		wm::ShowEvent show(
			window,
			event.type == MapNotify
			);
		dispatcher.dispatch(show);
	}
	
}

namespace wm
{
	namespace xlib
	{
		void dispatchXEvent(
			wm::Window& window,
			wm::common::Dispatcher &dispatcher,
			const XEvent &event
			)
		{
			typedef void (DispatchFunc)(
				wm::Window& window,
				wm::common::Dispatcher &,
				const XEvent &);

			static const struct Registry
			{
				Registry()
				{
					map[Expose] = dispatchExpose;
					map[ButtonPress] = dispatchButton;
					map[ButtonRelease] = dispatchButton;
					map[KeyPress] = dispatchKey;
					map[KeyRelease] = dispatchKey;
					map[FocusIn] = dispatchFocus;
					map[FocusOut] = dispatchFocus;
					map[EnterNotify] = dispatchMouseOver;
					map[LeaveNotify] = dispatchMouseOver;
					map[ResizeRequest] = dispatchResize;
					map[MapNotify] = dispatchShow;
					map[UnmapNotify] = dispatchShow;
				}
			
				typedef std::map<int, DispatchFunc*> map_t;
				map_t map;
			} registry;
				
			Registry::map_t::const_iterator iter =
				registry.map.find(event.type);
			if(iter != registry.map.end())
			{
				DispatchFunc *func = iter->second;
				func(window, dispatcher, event);
			} else
			{
				std::cout << "Unhandled event " << event_names[event.type] << std::endl;
			}
		}
	}
}

