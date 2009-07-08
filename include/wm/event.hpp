#ifndef EVENT_HPP
#define EVENT_HPP

namespace wm
{
	class Window;
	class EventHandler;

	/// An event object
	/**
		The event objects are accessible using the Visitor pattern.
	*/
	class Event
	{
		public:
			/// Destroy an Event object
			virtual ~Event() {}
			
			/// Visitor pattern entry point
			/**
				Calls EventHandler.handle(*this).
				
				@param handler the EventHandler to call
			*/
			virtual void accept(EventHandler& handler) const = 0;
			
			/// Get the Window the event originated from
			/**
				@return a reference to the Window object corresponding to the source of the event
			*/			
			Window &window() const { return *window_; }
			
		protected:
			/// Create a new Event
			/**
				@param window the Window object corresponding to the source of this event
			*/
			explicit Event(Window& window) : window_(&window) {}
		private:
			Window *window_;
	};
}

#endif

