#ifndef WM_EVENTS_EXPOSEEVENT_HPP
#define WM_EVENTS_EXPOSEEVENT_HPP

#include <wm/event.hpp>

namespace wm
{
	class ExposeEvent : public Event
	{
		public:
			ExposeEvent(
				Window& window,
				unsigned int x,
				unsigned int y,
				unsigned int width,
				unsigned int height)
				: Event(window)
				, x_(x)
				, y_(y)
				, width_(width)
				, height_(height)
			{
			}
			
			unsigned int x() const { return x_; }
			unsigned int y() const { return y_; }
			unsigned int width() const { return width_; }
			unsigned int height() const { return height_; }
			
			virtual void accept(EventHandler &handler) const { handler.handle(*this); }
			
		private:
			unsigned int x_, y_, width_, height_;
	};
}

#endif
