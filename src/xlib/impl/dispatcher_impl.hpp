#ifndef DISPATCHER_IMPL_HPP
#define DISPATCHER_IMPL_HPP

#include <X11/Xlib.h>

namespace wm
{
	class Window;
	class EventHandler;

	namespace common
	{
		class Dispatcher;
	}

	namespace xlib
	{
		void dispatchXEvent(
			wm::Window& window,
			wm::common::Dispatcher &dispatcher,
			const XEvent &event
			);
	}
}

#endif

