#ifndef WM_EVENTS_KEYEVENT_HPP
#define WM_EVENTS_KEYEVENT_HPP

#include <wm/event.hpp>
#include <wm/eventhandler.hpp>
#include <wm/keyboard.hpp>

#include <wm/export.hpp>

namespace wm
{
	/// A keyboard event
	/**
		A keyboard event occurs when the user presses or releases a
		key on the keyboard.
	*/
	class KeyEvent : public Event
	{
		public:
			/// Create a new key event
			/**
				@param window the Window object that recieved the key event
				@param symbol the key Symbol
				@param keymod keyboard modifier state mask 
				@param state true if the key was pressed, false if released
			*/
			KeyEvent(
				Window &window,
				keyboard::Symbol symbol,
				keyboard::KeyMod keymod,
				bool state)
				: Event(window)
				, symbol_(symbol)
				, keymod_(keymod)
				, state_(state)
			{
			}
			
			/// The key symbol
			/**
				@return the key symbol of the pressed/released key
			*/
			keyboard::Symbol symbol() const { return symbol_; }
			
			/// The state of the key
			/**
				@return true if the key was pressed, false if released			
			*/			
			bool state() const { return state_; }
			
			/// State of keyboard modifiers
			/**
				@return bitmask of keyboard modifiers
			*/
			keyboard::KeyMod keymod() const { return keymod_; }

			/// Visitor pattern entry point
			virtual void accept(EventHandler &handler) const { handler.handle(*this); }
			
		private:
			keyboard::Symbol symbol_;
			keyboard::KeyMod keymod_;
			bool state_;
	};
}

#undef WM_EXPORT

#endif
