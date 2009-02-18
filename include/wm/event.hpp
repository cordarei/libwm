#ifndef EVENT_HPP
#define EVENT_HPP

namespace wm
{
	class Window;
	class EventHandler;

	class Event
	{
		public:
			virtual ~Event() {}
			
			virtual void accept(EventHandler& handler) const = 0;
			
			Window &window() { return *window_; }
			const Window &window() const { return *window_; }
			
		protected:
			explicit Event(Window& window) : window_(&window) {}
		private:
			Window *window_;
	};
}

#include <wm/eventhandler.hpp>

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
	/*
	class KeyEvent : public Event
	{
	};
	
	class MotionEvent : public Event
	{
	};
	
	class ButtonEvent : public Event
	{
	}; */
}

#endif

