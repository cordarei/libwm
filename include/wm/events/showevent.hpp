#ifndef WM_EVENTS_SHOWEVENT_HPP
#define WM_EVENTS_SHOWEVENT_HPP

#include <wm/event.hpp>
#include <wm/eventhandler.hpp>

namespace wm
{
	/// A show event
	/**
		A show event occurs when the window is shown or hidden
		using Window::show or Window::hide or 
		
		@see Window::show
		@see Window::hide
	*/
	class ShowEvent : public Event
	{
		public:
			/// Create a new show event
			/**
				@param window the Window object corresponding to the shown/hidden window
				@param state true if the window is shown, false if hidden
			*/
			ShowEvent(
				Window &window,
				bool state)
				: Event(window)
				, state_(state)
			{
			}

			/// The state of the window
			/**
				@return true if the window is shown, false if hidden
			*/
			bool state() const { return state_; }

			/// Visitor pattern entry point
			virtual void accept(EventHandler &handler) const { handler.handle(*this); }
			
		private:
			bool state_;
	};
}

#endif
