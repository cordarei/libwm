#ifndef WM_EVENTS_BUTTONEVENT_HPP
#define WM_EVENTS_BUTTONEVENT_HPP

#include <wm/event.hpp>
#include <wm/eventhandler.hpp>

namespace wm
{
	class ButtonEvent : public Event
	{
		public:
			ButtonEvent(
				Window& window,
				unsigned int x,
				unsigned int y,
				int button,
				bool state)
				: Event(window)
				, x_(x)
				, y_(y)
				, button_(button)
				, state_(state)
			{
			}
			
			unsigned int x() const { return x_; }
			unsigned int y() const { return y_; }
			int button() const { return button_; }
			bool state() const { return state_; }
			
			virtual void accept(EventHandler &handler) const { handler.handle(*this); }
		
		private:
			unsigned int x_, y_;
			int button_;
			bool state_;
	};
}

#endif
