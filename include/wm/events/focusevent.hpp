#ifndef WM_EVENTS_FOCUSEVENT_HPP
#define WM_EVENTS_FOCUSEVENT_HPP

#include <wm/event.hpp>
#include <wm/eventhandler.hpp>

namespace wm
{
	class FocusEvent : public Event
	{
		public:
			FocusEvent(
				Window &window,
				bool state)
				: Event(window)
				, state_(state)
			{
			}

			bool state() const { return state_; }

			virtual void accept(EventHandler &handler) const { handler.handle(*this); }
			
		private:
			bool state_;
	};
}

#endif
