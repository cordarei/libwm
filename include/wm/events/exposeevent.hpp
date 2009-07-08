#ifndef WM_EVENTS_EXPOSEEVENT_HPP
#define WM_EVENTS_EXPOSEEVENT_HPP

#include <wm/event.hpp>

namespace wm
{
	/// An expose event
	/**
		An expose event occurs when a part of a window should be
		redrawn.
		
		A window with no animation should only be drawn when an
		expose event arrives (or when the content should be changed).
	*/
	class ExposeEvent : public Event
	{
		public:
			/// Create a new expose event
			/**
				@param window the Window object corresponding to the exposed window			
				@param x the X coordinate of the upper left corner of the exposed window area
				@param y the Y coordinate of the upper left corner of the exposed window area
				@param width the width of the exposed window area
				@param height the height of the exposed window area				
			*/
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
			
			/// the X coordinate of the upper left corner of the exposed window area
			/**
				@return the X coordinate of the upper left corner of the exposed window area
			*/
			unsigned int x() const { return x_; }
			
			/// the Y coordinate of the upper left corner of the exposed window area
			/**
				@return the Y coordinate of the upper left corner of the exposed window area
			*/
			unsigned int y() const { return y_; }
			
			/// the width of the exposed window area
			/**
				@return the width of the exposed window area
			*/
			unsigned int width() const { return width_; }

			/// the height of the exposed window area
			/**
				@return the height of the exposed window area
			*/
			unsigned int height() const { return height_; }
			
			/// Visitor pattern entry point
			virtual void accept(EventHandler &handler) const { handler.handle(*this); }
			
		private:
			unsigned int x_, y_, width_, height_;
	};
}

#endif
