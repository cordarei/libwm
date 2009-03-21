#include <map>

#include <X11/Xlib.h>
#include <X11/keysym.h>

#include <wm/events/keyevent.hpp>
#include "impl/keymap.hpp"

namespace
{
	KeySym reverse_keymap[wm::KeyEvent::NUM_KEYSYMS] = {
		0, 					// UNKNOWN
		0, 0, 0, 0, 0,		// 0x01 - 0x07 unused (total: 7)
		0, 0, 
		
		XK_BackSpace, 		// BACKSPACE
		XK_Tab,				// TAB
		XK_Linefeed, 		// LINEFEED
		XK_Clear,			// CLEAR
		
		0, 					// 0x0c unused
		
		XK_Return, 			// RETURN,
		
		0, 0, 0, 0, 0,		// 0x0e - 0x1a unused (total: 13)
		0, 0, 0, 0, 0,
		0, 0, 0, 			
		
		XK_Escape, 			// ESCAPE
		
		0, 0, 0, 0, 		// 0x1c - 0x1f unused (total: 4)
		
		XK_space,			// SPACE
		XK_exclam,			// EXCLAIM
		XK_quotedbl,		// DOUBLEQUOTE
		XK_numbersign,		// HASH
		XK_dollar,			// DOLLAR
		
		0,					// 0x25 unused
		
		XK_ampersand, 		// AMPERSAND
		XK_apostrophe,		// QUOTE
		XK_parenleft,		// LEFTPAREN
		XK_parenright,		// RIGHTPAREN
		XK_asterisk, 		// ASTERISK
		XK_plus,			// PLUS
		XK_comma,			// COMMA
		XK_minus,			// MINUS
		XK_period,			// PERIOD
		XK_slash, 			// SLASH

		XK_0,				// KEY_0
		XK_1,				// KEY_1
		XK_2,				// KEY_2
		XK_3,				// KEY_3
		XK_4,				// KEY_4
		XK_5,				// KEY_5
		XK_6,				// KEY_6
		XK_7,				// KEY_7
		XK_8,				// KEY_8
		XK_9,				// KEY_9
		
		XK_colon,			// COLON 
		XK_semicolon, 		// SEMICOLON 
		XK_less,			// LESS 
		XK_equal,			// EQUALS 
		XK_greater,			// GREATER 
		XK_question,		// QUESTION 
		XK_at,				// AT 

		XK_a,				// KEY_A
		XK_b,				// KEY_B
		XK_c,				// KEY_C
		XK_d,				// KEY_D
		XK_e,				// KEY_E
		XK_f,				// KEY_F
		XK_g,				// KEY_G
		XK_h,				// KEY_H
		XK_i,				// KEY_I
		XK_j,				// KEY_J
		XK_k,				// KEY_K
		XK_l,				// KEY_L
		XK_m,				// KEY_M
		XK_n,				// KEY_N
		XK_o,				// KEY_O
		XK_p,				// KEY_P
		XK_q,				// KEY_Q
		XK_r,				// KEY_R
		XK_s,				// KEY_S
		XK_t,				// KEY_T
		XK_u,				// KEY_U
		XK_v,				// KEY_V
		XK_w,				// KEY_W
		XK_x,				// KEY_X
		XK_y,				// KEY_Y
		XK_z,				// KEY_Z
		
		XK_bracketleft,		// LEFTBRACKET 
		XK_backslash,		// BACKSLASH 
		XK_bracketright,	// RIGHTBRACKET 
		XK_asciicircum,		// CARET 
		XK_underscore,		// UNDERSCORE 
		XK_grave,			// BACKQUOTE
		
		/* 0x61 - 0x7e unused (total: 30) */
		
		XK_Delete,			// DELETE 
		
		// END OF ASCII
		
		0					// TERMINATOR
		};

	typedef std::map<KeySym, wm::KeyEvent::Symbol> Keymap;
	
	const struct Mapper
	{
		Mapper()
		{
			typedef unsigned int uint;
			for(uint i = 0;
				i < uint(wm::KeyEvent::NUM_KEYSYMS);
				++i)
			{
				KeySym sym = reverse_keymap[i];
				if(sym) map[sym] = wm::KeyEvent::Symbol(i);
			}
		}
		
		Keymap map;
	} mapper;
}

namespace wm
{
	namespace xlib
	{
		wm::KeyEvent::Symbol mapXKeySym(KeySym sym)
		{
			const Keymap &map = mapper.map;
			Keymap::const_iterator i = map.find(sym);
			if(i == map.end()) return wm::KeyEvent::UNKNOWN;
			return i->second;
		}
	}
}

