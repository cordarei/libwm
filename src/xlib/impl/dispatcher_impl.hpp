#ifndef DISPATCHER_IMPL_HPP
#define DISPATCHER_IMPL_HPP

#include <list>

#include <X11/Xlib.h>

namespace wm
{
	class Window;
	class EventHandler;

	namespace xlib
	{
		void dispatchXEvent(
			wm::Window& window,
			const std::list<EventHandler*> &handlers,
			const XEvent &event
			);
	}
}

#endif

