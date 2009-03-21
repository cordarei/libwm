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
		
		/* 0x61 - 0x7e unused (total: 30) */
		
		VK_DELETE,			// DELETE 
		
		// END OF ASCII
		
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
	}
}

