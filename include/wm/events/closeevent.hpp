#ifndef WM_EVENTS_CLOSEEVENT_HPP
#define WM_EVENTS_CLOSEEVENT_HPP

#include <wm/event.hpp>
#include <wm/eventhandler.hpp>

namespace wm
{
	class CloseEvent : public Event
	{
		public:
			CloseEvent(
				Window &window)
				: Event(window)
			{
			}

			virtual void accept(EventHandler &handler) const { handler.handle(*this); }
	};
}

#endif
