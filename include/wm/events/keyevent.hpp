#ifndef WM_EVENTS_KEYEVENT_HPP
#define WM_EVENTS_KEYEVENT_HPP

#include <wm/event.hpp>
#include <wm/eventhandler.hpp>

#include <wm/export.hpp>

namespace wm
{
	class KeyEvent : public Event
	{
		public:
			enum Symbol {
				UNKNOWN = 0,
				
				// 0x01 - 0x07 unused (total: 7)
				
				BACKSPACE = 0x08,
				TAB = 0x09,
				LINEFEED = 0x0a,
				CLEAR = 0x0b,
				
				// 0x0c unused
				
				RETURN = 0x0d,
				
				// 0x0e - 0x1a unused (total: 13)
				
				ESCAPE = 0x1b,
				
				// 0x1c - 0x1f unused (total: 4)
				
				SPACE = 0x20,
				EXCLAIM		= 0x21,
				DOUBLEQUOTE = 0x22,
				HASH		= 0x23,
				DOLLAR		= 0x24,
				
				// 0x25 unused
				
				AMPERSAND		= 0x26,
				QUOTE		= 0x27,
				LEFTPAREN		= 0x28,
				RIGHTPAREN		= 0x29,
				ASTERISK		= 0x2a,
				PLUS		= 0x2b,
				COMMA		= 0x2c,
				MINUS		= 0x2d,
				PERIOD		= 0x2e,
				SLASH		= 0x2f,

				KEY_0 = 0x30,
				KEY_1 = 0x31,
				KEY_2 = 0x32,
				KEY_3 = 0x33,
				KEY_4 = 0x34,
				KEY_5 = 0x35,
				KEY_6 = 0x36,
				KEY_7 = 0x37,
				KEY_8 = 0x38,
				KEY_9 = 0x39,
				
				COLON = 0x3a,
				SEMICOLON = 0x3b,
				LESS = 0x3c,
				EQUALS = 0x3d,
				GREATER = 0x3e,
				QUESTION = 0x3f,
				AT = 0x40,
				
				KEY_A = 0x41,
				KEY_B = 0x42,
				KEY_C = 0x43,
				KEY_D = 0x44,
				KEY_E = 0x45,
				KEY_F = 0x46,
				KEY_G = 0x47,
				KEY_H = 0x48,
				KEY_I = 0x49,
				KEY_J = 0x4a,
				KEY_K = 0x4b,
				KEY_L = 0x4c,
				KEY_M = 0x4d,
				KEY_N = 0x4e,
				KEY_O = 0x4f,
				KEY_P = 0x50,
				KEY_Q = 0x51,
				KEY_R = 0x52,
				KEY_S = 0x53,
				KEY_T = 0x54,
				KEY_U = 0x55,
				KEY_V = 0x56,
				KEY_W = 0x57,
				KEY_X = 0x58,
				KEY_Y = 0x59,
				KEY_Z = 0x5a,
				
				LEFTBRACKET = 0x5b,
				BACKSLASH = 0x5c,
				RIGHTBRACKET = 0x5d,
				CARET = 0x5e,
				UNDERSCORE = 0x5f,
				BACKQUOTE = 0x60,
				
				// 0x61 - 0x7e unused (total: 30)
				
				DELETE = 0x7f,
				
				// END OF ASCII
				
				NUM_KEYSYMS // TERMINATOR
				};
		
		
			KeyEvent(
				Window &window,
				Symbol symbol,
				bool state)
				: Event(window)
				, symbol_(symbol)
				, state_(state)
			{
			}
			
			Symbol symbol() const { return symbol_; }
			bool state() const { return state_; }

			virtual void accept(EventHandler &handler) const { handler.handle(*this); }
			
		private:
			Symbol symbol_;
			bool state_;
	};
	
	const char WM_EXPORT *keyName(wm::KeyEvent::Symbol);
}

#undef WM_EXPORT

#endif
