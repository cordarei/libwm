#ifndef EVENTFACTORY_IMPL_HPP
#define EVENTFACTORY_IMPL_HPP

#include <X11/Xlib.h>

namespace wm
{
	class Window;
	class Event;
	
	namespace xlib
	{
		extern long event_mask;
	
		const Event* fromXEvent(
			wm::Window& window,
			const XEvent &event,
			bool filter
			);
	}
}

#endif

