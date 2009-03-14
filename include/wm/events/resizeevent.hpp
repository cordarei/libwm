#ifndef WM_EVENTS_RESIZEEVENT_HPP
#define WM_EVENTS_RESIZEEVENT_HPP

#include <wm/event.hpp>
#include <wm/eventhandler.hpp>

namespace wm
{
	class ResizeEvent : public Event
	{
		public:
			ResizeEvent(
				Window& window,
				unsigned int width,
				unsigned int height)
				: Event(window)
				, width_(width)
				, height_(height)
			{
			}
			
			unsigned int width() const { return width_; }
			unsigned int height() const { return height_; }
			
			virtual void accept(EventHandler &handler) const { handler.handle(*this); }
			
		private:
			unsigned int width_, height_;
	};
}

#endif
