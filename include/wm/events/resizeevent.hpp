#ifndef WM_EVENTS_RESIZEEVENT_HPP
#define WM_EVENTS_RESIZEEVENT_HPP

#include <wm/event.hpp>
#include <wm/eventhandler.hpp>

namespace wm
{
	/// A resize event
	/**
		A resize event occurs when the user resizes a window or
		the window is explicitly resized using Window::resize or 
		the window is resized by the windowing system.
		
		@see Window::resize
		@see Window::getSize
	*/
	class ResizeEvent : public Event
	{
		public:
			/// Create a new resize event
			/**
				@param window the Widow object corresponding to the resized window
				@param width the new width of the window
				@param height the new height of the window
			*/
			ResizeEvent(
				Window& window,
				unsigned int width,
				unsigned int height)
				: Event(window)
				, width_(width)
				, height_(height)
			{
			}
			
			/// The new width of the window
			/**
				@return the new width of the window
			*/
			unsigned int width() const { return width_; }

			/// The new height of the window
			/**
				@return the new height of the window
			*/
			unsigned int height() const { return height_; }
			
			/// Visitor pattern entry point
			virtual void accept(EventHandler &handler) const { handler.handle(*this); }
			
		private:
			unsigned int width_, height_;
	};
}

#endif
