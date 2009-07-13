#ifndef WM_EVENTS_MOTIONEVENT_HPP
#define WM_EVENTS_MOTIONEVENT_HPP

#include <wm/event.hpp>

namespace wm
{
	/// A mouse motion event
	/**
		A mouse motion event occurs when the user moves the mouse pointer
		inside the window client area.	
	*/
	class MotionEvent : public Event
	{
		public:
			/// Create a new MotionEvent
			/**
				@param window the window where the pointer moved
				@param x the x-coordinate of the new pointer position
				@param y the y-coordinate of the new pointer position
			*/
			MotionEvent(Window& window, unsigned int x, unsigned int y)
				: Event(window)
				, x_(x)
				, y_(y)
			{
			}
			
			/// Get the horizontal pointer position
			/**
				@return the x-coordinate of the new pointer position
			*/
			unsigned int x() const { return x_; }

			/// Get the vertical pointer position
			/**
				@return the y-coordinate of the new pointer position
			*/
			unsigned int y() const { return y_; }
			
			/// Visitor pattern entry point
			virtual void accept(EventHandler &handler) const { handler.handle(*this); }
			
		private:
			unsigned int x_, y_;
	};
}

#endif

