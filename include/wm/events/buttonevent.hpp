#ifndef WM_EVENTS_BUTTONEVENT_HPP
#define WM_EVENTS_BUTTONEVENT_HPP

#include <wm/event.hpp>
#include <wm/eventhandler.hpp>

namespace wm
{
	/// A moouse button event
	/**
		A mouse button event occurs when the user presses a mouse button
		or scrolls the mouse wheel inside a Window's client area.
	*/
	class ButtonEvent : public Event
	{
		public:
			/// Create a new mouse button event
			/**
				@param window the Window objcet corresponding to the source of the event
				@param x the horizontal position of the mouse pointer when this event occured
				@param y the vertical position of the mouse pointer when this event occured
				@param button the number of the mouse button
				@param state true if the mouse button was pressed, false if released
			*/			
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
			
			/// The X coordinate of the click
			/**
				@return the horizontal position of the mouse pointer when this event occured
			*/
						
			unsigned int x() const { return x_; }

			/// The Y coordinate of the click
			/**
				@return the vertical position of the mouse pointer when this event occured
			*/
			unsigned int y() const { return y_; }
			
			/// Number of the mouse button
			/**
				@return the number of the mouse button
			*/
			int button() const { return button_; }
			
			/// The state of the button
			/**
				@return true if the mouse button was pressed, false if released
			*/			
			bool state() const { return state_; }
			
			/// Visitor pattern entry point
			virtual void accept(EventHandler &handler) const { handler.handle(*this); }
		
		private:
			unsigned int x_, y_;
			int button_;
			bool state_;
	};
}

#endif
