#include <iostream>
#include <list>
#include <map>

#include <boost/ref.hpp>
#include <boost/bind.hpp>

#include <X11/Xlib.h>

#include <wm/eventhandler.hpp>
#include <wm/event.hpp>

namespace 
{
	typedef std::list<wm::EventHandler*> HandlerList;
	typedef void (DispatchFunc)(wm::Window& window, const HandlerList&, const XEvent &);
	
	void dispatch(
		const HandlerList &handlers,
		const wm::Event &event
		)
	{
		using boost::bind;
		using boost::ref;
		
		for(HandlerList::const_iterator i = handlers.begin();
			i != handlers.end();
			++i)
		{
			wm::EventHandler& handler = **i;
			event.accept(handler);
		}

	}

	void dispatchExpose(
		wm::Window& window,
		const HandlerList &handlers,
		const XEvent &event
		)
	{
		wm::ExposeEvent expose(
			window,
			event.xexpose.x,
			event.xexpose.y,
			event.xexpose.width,
			event.xexpose.height);
		dispatch(handlers, expose);
	}

}

namespace wm
{
	namespace xlib
	{
		void dispatchXEvent(
			wm::Window& window,
			const std::list<EventHandler*> &handlers,
			const XEvent &event
			)
		{
			static const char *event_names[] = {
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
			
			static const struct Registry
			{
				Registry()
				{
					map[Expose] = dispatchExpose;
				}
			
				typedef std::map<int, DispatchFunc*> map_t;
				map_t map;
			} registry;
				
			Registry::map_t::const_iterator iter =
				registry.map.find(event.type);
			if(iter != registry.map.end())
			{
				DispatchFunc *func = iter->second;
				func(window, handlers, event);
			} else
			{
				std::cout << "Unhandled event " << event_names[event.type] << std::endl;
			}
		}
	}
}

