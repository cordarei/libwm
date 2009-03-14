#ifndef WM_EVENTS_MOUSEOVEREVENT_HPP
#define WM_EVENTS_MOUSEOVEREVENT_HPP

#include <wm/event.hpp>
#include <wm/eventhandler.hpp>

namespace wm
{
	class MouseOverEvent : public Event
	{
		public:
			MouseOverEvent(
				Window &window,
				int x,
				int y,
				bool inside)
				: Event(window)
				, x_(x)
				, y_(y)
				, inside_(inside)
			{
			}

			int x() const { return x_; }
			int y() const { return y_; }
			bool inside() const { return inside_; }

			virtual void accept(EventHandler &handler) const { handler.handle(*this); }
			
		private:
			int x_, y_;
			bool inside_;
	};
}

#endif
