#ifndef WM_EVENTS_MOUSEOVEREVENT_HPP
#define WM_EVENTS_MOUSEOVEREVENT_HPP

#include <wm/event.hpp>
#include <wm/eventhandler.hpp>

namespace wm
{
	/// A mouse over event
	/**
		Mouse over event occur when the mouse pointer enters
		or exits a window area.
	*/
	class MouseOverEvent : public Event
	{
		public:
			/// Create a new mouse over event
			/**
				@param window the Window object corresponding to the window that the pointer entered/left
				@param x the X coordinate of the pointer
				@param y the Y coordinate of the pointer
				@param inside true if the pointer entered the window, false if left
			*/
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

			/// Mouse pointer X coordinate
			/**
				@return the X coordinate of the pointer
			*/
			int x() const { return x_; }
			
			/// Mouse pointer Y coordinate
			/**
				@return the X coordinate of the pointer
			*/
			int y() const { return y_; }
			
			/// Is the pointer inside the window
			/**
				@return true if the pointer entered the window, false if left
			*/
			bool inside() const { return inside_; }

			/// Visitor pattern entry point
			virtual void accept(EventHandler &handler) const { handler.handle(*this); }
			
		private:
			int x_, y_;
			bool inside_;
	};
}

#endif
