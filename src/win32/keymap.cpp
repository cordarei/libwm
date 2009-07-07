#include <map>

#include <wm/events/keyevent.hpp>
#include "impl/keymap.hpp"

#include <windows.h>

namespace
{
	WPARAM reverse_keymap[wm::KeyEvent::NUM_KEYSYMS] = {
		0, 					// UNKNOWN
		0, 0, 0, 0, 0,		// 0x01 - 0x07 unused (total: 7)
		0, 0, 
		
		VK_BACK, 			// BACKSPACE
		VK_TAB,				// TAB
		0,		 			// LINEFEED
		VK_CLEAR,			// CLEAR
		
		0, 					// 0x0c unused
		
		VK_RETURN, 			// RETURN,
		
		0, 0, 0, 0, 0,		// 0x0e - 0x1a unused (total: 13)
		0, 0, 0, 0, 0,
		0, 0, 0, 			
		
		VK_ESCAPE, 			// ESCAPE
		
		0, 0, 0, 0, 		// 0x1c - 0x1f unused (total: 4)
		
		VK_SPACE,			// SPACE
		0,					// EXCLAIM
		0,					// DOUBLEQUOTE
		0,					// HASH
		0,					// DOLLAR
		
		0,					// 0x25 unused
		
		0, 					// AMPERSAND
		0,					// QUOTE
		0,					// LEFTPAREN
		0,					// RIGHTPAREN
		0, 					// ASTERISK
		0,					// PLUS
		0,					// COMMA
		0,					// MINUS
		0,					// PERIOD
		0, 					// SLASH

		'0',				// KEY_0
		'1',				// KEY_1
		'2',				// KEY_2
		'3',				// KEY_3
		'4',				// KEY_4
		'5',				// KEY_5
		'6',				// KEY_6
		'7',				// KEY_7
		'8',				// KEY_8
		'9',				// KEY_9
		
		0,					// COLON 
		0, 					// SEMICOLON 
		0,					// LESS 
		0,					// EQUALS 
		0,					// GREATER 
		0,					// QUESTION 
		0,					// AT 

		'A',				// KEY_A
		'B',				// KEY_B
		'C',				// KEY_C
		'D',				// KEY_D
		'E',				// KEY_E
		'F',				// KEY_F
		'G',				// KEY_G
		'H',				// KEY_H
		'I',				// KEY_I
		'J',				// KEY_J
		'K',				// KEY_K
		'L',				// KEY_L
		'M',				// KEY_M
		'N',				// KEY_N
		'O',				// KEY_O
		'P',				// KEY_P
		'Q',				// KEY_Q
		'R',				// KEY_R
		'S',				// KEY_S
		'T',				// KEY_T
		'U',				// KEY_U
		'V',				// KEY_V
		'W',				// KEY_W
		'X',				// KEY_X
		'Y',				// KEY_Y
		'Z',				// KEY_Z
		
		0,					// LEFTBRACKET 
		0,					// BACKSLASH 
		0,					// RIGHTBRACKET 
		0,					// CARET 
		0,					// UNDERSCORE 
		0,					// BACKQUOTE
		
		0, 0, 0, 0, 0,		// 0x61 - 0x7e unused (total: 30)
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		
		VK_DELETE,			// DELETE 
		
		// END OF ASCII

		0, 0, 0, 0, 0,		// 0x80 - 0x9f unused (total: 32)
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0,

		// International unicode characters 0xa0 - 0xff
		// Unused for now (total: 96)
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0,

		VK_NUMPAD0,			// KEYPAD_0
		VK_NUMPAD1,			// KEYPAD_1
		VK_NUMPAD2,			// KEYPAD_2
		VK_NUMPAD3,			// KEYPAD_3
		VK_NUMPAD4,			// KEYPAD_4
		VK_NUMPAD5,			// KEYPAD_5
		VK_NUMPAD6,			// KEYPAD_6
		VK_NUMPAD7,			// KEYPAD_7
		VK_NUMPAD8,			// KEYPAD_8
		VK_NUMPAD9,			// KEYPAD_9
		
		VK_DECIMAL, 		// KEYPAD_SEPARATOR
		VK_DIVIDE,		 	// KEYPAD_DIVIDE
		VK_MULTIPLY, 		// KEYPAD_MULTIPLY
		VK_ADD,				// KEYPAD_PLUS
		VK_SUBTRACT, 		// KEYPAD_MINUS
		0,					// KEYPAD_ENTER
		
		VK_UP,				// UP
		VK_DOWN, 			// DOWN
		VK_LEFT, 			// LEFT
		VK_RIGHT, 			// RIGHT
		VK_INSERT, 			// INSERT
		VK_HOME, 			// HOME
		VK_END, 			// END
		VK_PRIOR,			// PAGEUP
		VK_NEXT,			// PAGEDOWN
		
		VK_F1,				// F1
		VK_F2,				// F2
		VK_F3,				// F3
		VK_F4,				// F4
		VK_F5,				// F5
		VK_F6,				// F6
		VK_F7,				// F7
		VK_F8,				// F8
		VK_F9,				// F9
		VK_F10,				// F10
		VK_F11,				// F10
		VK_F12,				// F12
		VK_F13,				// F13
		VK_F14,				// F14
		VK_F15,				// F15
		VK_F16,				// F16
		VK_F17,				// F17
		VK_F18,				// F18
		VK_F19,				// F19
		VK_F20,				// F20
		VK_F21,				// F21
		VK_F22,				// F22
		VK_F23,				// F23
		VK_F24,				// F24
		
		VK_NUMLOCK, 		// NUMLOCK
		VK_CAPITAL, 		// CAPSLOCK
		VK_SCROLL,		 	// SCROLLLOCK
		VK_SHIFT, 			// LSHIFT
		0, 					// RSHIFT
		VK_CONTROL, 		// LCTRL
		0, 					// RCRTL
		VK_MENU, 			// LALT
		0,		 			// RALT
		VK_LWIN,	 		// LSUPER
		VK_RWIN, 			// RSUPER
		
		0,					// ALTGR
		0,					// COMPOSE
		VK_RMENU,			// MENU
		
		0,					// SYS_REQ
		VK_PAUSE,			// PAUSE
		VK_SNAPSHOT,		// PRINT

		0, 					// DEAD_GRAVE
		0,			 		// DEAD_ACUTE
		0,					// DEAD_CIRCUMFLEX
		0,			 		// DEAD_TILDE
		0, 					// DEAD_MACRON
		0,					// DEAD_BREVE
		0,					// DEAD_ABOVEDOT
		0,					// DEAD_DIAERESIS
		0,					// DEAD_ABOVERING

		0					// TERMINATOR
		};

	typedef std::map<WPARAM, wm::KeyEvent::Symbol> Keymap;
	
	const struct Mapper
	{
		Mapper()
		{
			typedef unsigned int uint;
			for(uint i = 0;
				i < uint(wm::KeyEvent::NUM_KEYSYMS);
				++i)
			{
				WPARAM wparam = reverse_keymap[i];
				if(wparam) map[wparam] = wm::KeyEvent::Symbol(i);
			}
		}
		
		Keymap map;
	} mapper;
}

namespace wm
{
	namespace win32
	{
		wm::KeyEvent::Symbol mapVirtualKeyCode(WPARAM vCode)
		{
			const Keymap &map = mapper.map;
			Keymap::const_iterator i = map.find(vCode);
			if(i == map.end()) return wm::KeyEvent::UNKNOWN;
			return i->second;
		}

		wm::KeyEvent::Symbol translateKeyEvent(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam, bool &filter)
		{
			filter = false;

			if ( !(HIWORD(lparam) & 0x100) )	// Handle Keypad and Numlock
			{
				UINT mapped = MapVirtualKeyW(HIWORD(lparam) & 0xFF, MAPVK_VSC_TO_VK);

				switch(mapped)
				{
					case VK_INSERT:     return KeyEvent::KEYPAD_0;
					case VK_END:        return KeyEvent::KEYPAD_1;
					case VK_DOWN:       return KeyEvent::KEYPAD_2;
					case VK_NEXT:       return KeyEvent::KEYPAD_3;
					case VK_LEFT:       return KeyEvent::KEYPAD_4;
					case VK_CLEAR:      return KeyEvent::KEYPAD_5;
					case VK_RIGHT:      return KeyEvent::KEYPAD_6;
					case VK_HOME:       return KeyEvent::KEYPAD_7;
					case VK_UP:         return KeyEvent::KEYPAD_8;
					case VK_PRIOR:      return KeyEvent::KEYPAD_9;
					case VK_DIVIDE:     return KeyEvent::KEYPAD_DIVIDE;
					case VK_MULTIPLY:   return KeyEvent::KEYPAD_MULTIPLY;
					case VK_SUBTRACT:   return KeyEvent::KEYPAD_MINUS;
					case VK_ADD:        return KeyEvent::KEYPAD_PLUS;
					case VK_DELETE:     return KeyEvent::KEYPAD_SEPARATOR;
					default:			break;
				}
			}

			if(wparam == VK_SHIFT)
			{
				UINT scancode = MapVirtualKeyW(VK_RSHIFT, MAPVK_VK_TO_VSC);
				if(((lparam & 0x01ff0000) >> 16) == scancode)
					return KeyEvent::RSHIFT;
				return KeyEvent::LSHIFT;
			} else if(wparam == VK_CONTROL)
			{
				if(lparam & 0x01000000)
					return KeyEvent::RCRTL;

				if(message == WM_SYSKEYUP)
				{
					// WM_SYSKEYUP following the release of AltGR
					filter = true;
					return wm::KeyEvent::UNKNOWN;
				}

				// Detect AltGR
				LONG msgtime = GetMessageTime();
				MSG msg;
				if(PeekMessage(&msg, hwnd, WM_KEYFIRST, WM_KEYLAST, PM_NOREMOVE))
				{
					if((msg.message == WM_KEYDOWN || msg.message == WM_SYSKEYDOWN) &&
						msg.wParam == VK_MENU &&
						(msg.lParam & 0x01000000) &&
						msg.time == msgtime)
					{
						filter = true;
						return KeyEvent::UNKNOWN; // next message is RALT down
					}
				}

				return KeyEvent::LCTRL;
			} else if(wparam == VK_MENU)
			{
				if(lparam & 0x01000000)
					return KeyEvent::RALT;
				return KeyEvent::LALT;
			} else if(wparam == VK_RETURN)
			{
				if(lparam & 0x01000000)
					return KeyEvent::KEYPAD_ENTER;
				return KeyEvent::RETURN;
			}

			UINT charcode = MapVirtualKeyW(wparam, MAPVK_VK_TO_CHAR);
			if((charcode >= 0x20 && charcode < 0x40) || // some ASCII characters
				(charcode >= 0xa0 && charcode <= 0xff)) // Latin-1 supplement
				return static_cast<KeyEvent::Symbol>(charcode);

			return mapVirtualKeyCode(wparam);
		}
	}
}

