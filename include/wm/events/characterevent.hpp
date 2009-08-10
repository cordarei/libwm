#ifndef WM_EVENTS_CHARACTEREVENT_HPP
#define WM_EVENTS_CHARACTEREVENT_HPP

#include <wm/event.hpp>
#include <wm/eventhandler.hpp>

namespace wm
{
	/// A text input event
	/**
		A character event occurs when the user has typed some text input,
		typically when pressing one or more keys on the keyboard.
	*/
	class CharacterEvent : public Event
	{
		public:
			/// An integer big enough to handle all Unicode code points
			typedef unsigned int Unicode;
		
			/// Create a new CharacterEvent
			CharacterEvent(
				Window &window,
				Unicode unicode)
				: Event(window)
				, unicode_(unicode)
			{
			}

			/// Visitor pattern entry point
			virtual void accept(EventHandler &handler) const { handler.handle(*this); }

			/// The Unicode code point
			/**
				@return the typed Unicode code point
			*/
			Unicode unicode() const { return unicode_; }

		private:
			Unicode unicode_;
	};
}

#endif
