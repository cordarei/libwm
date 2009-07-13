#ifndef WM_EVENTS_MOTIONEVENT_HPP
#define WM_EVENTS_MOTIONEVENT_HPP

#include <wm/event.hpp>
#include <wm/mouse.hpp>
#include <wm/keyboard.hpp>

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
				@param buttons mouse button state mask
				@param keymod keyboard modifier state mask 
			*/
			MotionEvent(
				Window& window,
				unsigned int x,
				unsigned int y,
				mouse::ButtonMask buttons,
				keyboard::KeyMod keymod)
				: Event(window)
				, x_(x)
				, y_(y)
				, buttons_(buttons)
				, keymod_(keymod)
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

			/// State of mouse buttons
			/**
				@return bitmask of mouse buttons
			*/
			mouse::ButtonMask buttons() const { return buttons_; }
			
			/// State of keyboard modifiers
			/**
				@return bitmask of keyboard modifiers
			*/
			keyboard::KeyMod keymod() const { return keymod_; }

			/// Visitor pattern entry point
			virtual void accept(EventHandler &handler) const { handler.handle(*this); }
			
		private:
			unsigned int x_, y_;
			mouse::ButtonMask buttons_;
			keyboard::KeyMod keymod_;
	};
}

#endif

