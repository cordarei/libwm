#include <wm/events/keyevent.hpp>

namespace
{
	const char* keynames[wm::KeyEvent::NUM_KEYSYMS] = {
		"Unknown", 					// UNKNOWN
		0, 0, 0, 0, 0,		// 0x01 - 0x07 unused (total: 7)
		0, 0, 
		
		"Backspace", 		// BACKSPACE
		"Tab",				// TAB
		"Line feed", 		// LINEFEED
		"Clear",			// CLEAR
		
		0, 					// 0x0c unused
		
		"Return", 			// RETURN,
		
		0, 0, 0, 0, 0,		// 0x0e - 0x1a unused (total: 13)
		0, 0, 0, 0, 0,
		0, 0, 0, 			
		
		"Escape", 			// ESCAPE
		
		0, 0, 0, 0, 		// 0x1c - 0x1f unused (total: 4)
		
		"Space",			// SPACE
		"Exclamation",		// EXCLAIM
		"Double quote",		// DOUBLEQUOTE
		"Hash",				// HASH
		"Dollar",			// DOLLAR
		
		0,					// 0x25 unused
		
		"Ampersand", 		// AMPERSAND
		"Quote",			// QUOTE
		"Left paren",		// LEFTPAREN
		"Right paren",		// RIGHTPAREN
		"Asterisk", 		// ASTERISK
		"Plus",				// PLUS
		"Comma",			// COMMA
		"Minus",			// MINUS
		"Period",			// PERIOD
		"Slash", 			// SLASH

		"0",				// KEY_0
		"1",				// KEY_1
		"2",				// KEY_2
		"3",				// KEY_3
		"4",				// KEY_4
		"5",				// KEY_5
		"6",				// KEY_6
		"7",				// KEY_7
		"8",				// KEY_8
		"9",				// KEY_9
		
		"Colon",			// COLON 
		"Semicolon", 		// SEMICOLON 
		"Less",				// LESS 
		"Equals",			// EQUALS 
		"Greater",			// GREATER 
		"Question",			// QUESTION 
		"@",				// AT 

		"A",				// KEY_A
		"B",				// KEY_B
		"C",				// KEY_C
		"D",				// KEY_D
		"E",				// KEY_E
		"F",				// KEY_F
		"G",				// KEY_G
		"H",				// KEY_H
		"I",				// KEY_I
		"J",				// KEY_J
		"K",				// KEY_K
		"L",				// KEY_L
		"M",				// KEY_M
		"N",				// KEY_N
		"O",				// KEY_O
		"P",				// KEY_P
		"Q",				// KEY_Q
		"R",				// KEY_R
		"S",				// KEY_S
		"T",				// KEY_T
		"U",				// KEY_U
		"V",				// KEY_V
		"W",				// KEY_W
		"X",				// KEY_X
		"Y",				// KEY_Y
		"Z",				// KEY_Z
		
		"Left bracket",		// LEFTBRACKET 
		"Backslash",		// BACKSLASH 
		"Right bracket",	// RIGHTBRACKET 
		"Caret",			// CARET 
		"Underscore",		// UNDERSCORE 
		"Backquote",		// BACKQUOTE
		
		/* 0x61 - 0x7e unused (total: 30) */
		
		"Delete",			// DELETE 
		
		// END OF ASCII
		
		0					// TERMINATOR
		};
}

namespace wm
{
	const char *keyName(wm::KeyEvent::Symbol symbol)
	{
		if(symbol < wm::KeyEvent::NUM_KEYSYMS && keynames[symbol] != 0) return keynames[symbol];
		return "";
	}
}

