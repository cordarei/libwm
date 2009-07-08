#ifndef WM_EVENTS_FOCUSEVENT_HPP
#define WM_EVENTS_FOCUSEVENT_HPP

#include <wm/event.hpp>
#include <wm/eventhandler.hpp>

namespace wm
{
	/// Keyboard focus event
	/**
		A keyboard focus event arrives when the user
		switches the active focus window. When a Window
		has lost focus, it cannot recieve any keyboard events.
		
		An application may handle the focus events, for example
		pause a running game, or just ignore them.
	*/
	class FocusEvent : public Event
	{
		public:
			/// Create a new focus event
			/**
				@param window the window that got or lost focus
				@param state true if the window got focus, false if lost
			*/
			FocusEvent(
				Window &window,
				bool state)
				: Event(window)
				, state_(state)
			{
			}

			/// 
			/**
				@return true if the window got focus, false if lost
			*/
			bool state() const { return state_; }

			/// Visitor pattern entry point
			virtual void accept(EventHandler &handler) const { handler.handle(*this); }
			
		private:
			bool state_;
	};
}

#endif
