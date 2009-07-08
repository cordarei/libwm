#ifndef WM_EVENTS_CLOSEEVENT_HPP
#define WM_EVENTS_CLOSEEVENT_HPP

#include <wm/event.hpp>
#include <wm/eventhandler.hpp>

namespace wm
{
	/// A window closing event
	/**
		A close event occurs when the user closes a Window by clicking
		the close button in the title bar, pressing a key combination
		such as ALT-F4 or closing the window from the task bar or by
		other means.
	*/
	class CloseEvent : public Event
	{
		public:
			/// Create a new close event
			/**
				@param window the Window object corresponding to the closed window
			*/
			CloseEvent(
				Window &window)
				: Event(window)
			{
			}

			/// Visitor pattern entry point
			virtual void accept(EventHandler &handler) const { handler.handle(*this); }
	};
}

#endif
