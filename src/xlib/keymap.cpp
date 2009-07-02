#include <map>

#include <X11/Xlib.h>
#include <X11/keysym.h>

#include <wm/events/keyevent.hpp>
#include "impl/keymap.hpp"

namespace
{
	KeySym reverse_keymap[wm::KeyEvent::NUM_KEYSYMS+1] = {
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
		
		0, 0, 0, 0, 0,		// 0x61 - 0x7e unused (total: 30)
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,

		XK_Delete,			// DELETE 
		
		// END OF ASCII
		
		0, 0, 0, 0, 0,		// 0x80 - 0x9f unused (total: 32)
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0,

		// International unicode characters 0xa0 - 0xff
		XK_nobreakspace,	// NOBREAKSPACE (0xa0)
		XK_exclamdown,		// EXCLAMDOWN (0xa1)
		XK_cent,			// CENT (0xa2)
		XK_sterling,		// STERLING (0xa3)
		XK_currency,		// CURRENCY (0xa4)
		XK_yen,				// YEN (0xa5)
		XK_brokenbar,		// BROKENBAR (0xa6)
		XK_section,			// SECTION (0xa7)
		XK_diaeresis,		// DIAERESIS (0xa8)
		XK_copyright,		// COPYRIGHT (0xa9)
		XK_ordfeminine,		// ORDFEMININE (0xaa)
		XK_guillemotleft,	// GUILLEMOTLEFT (0xab)
		XK_notsign,			// NOTSIGN (0xac)
		XK_hyphen,			// HYPHEN (0xad)
		XK_registered,		// REGISTERED (0xae)
		XK_macron,			// MACRON (0xaf)
		XK_degree,			// DEGREE (0xb0)
		XK_plusminus,		// PLUSMINUS (0xb1)
		XK_twosuperior,		// TWOSUPERIOR (0xb2)
		XK_threesuperior,	// THREESUPERIOR (0xb3)
		XK_acute,			// ACUTE (0xb4)
		XK_mu,				// MU (0xb5)
		XK_paragraph,		// PARAGRAPH (0xb6)
		XK_periodcentered,	// PERIODCENTERED (0xb7)
		XK_cedilla,			// CEDILLA (0xb8)
		XK_onesuperior,		// ONESUPERIOR (0xb9)
		XK_masculine,		// MASCULINE (0xba)
		XK_guillemotright,	// GUILLEMOTRIGHT (0xbb)
		XK_onequarter,		// ONEQUARTER (0xbc)
		XK_onehalf,			// ONEHALF (0xbd)
		XK_threequarters,	// THREEQUARTERS (0xbe)
		XK_questiondown,	// QUESTIONDOWN (0xbf)
		XK_Agrave,			// AGRAVE (0xc0)
		XK_Aacute,			// AACUTE (0xc1)
		XK_Acircumflex,		// ACIRCUMFLEX (0xc2)
		XK_Atilde,			// ATILDE (0xc3)
		XK_Adiaeresis,		// ADIAERESIS (0xc4)
		XK_Aring,			// ARING (0xc5)
		XK_AE,				// AE (0xc6)
		XK_Ccedilla,		// CCEDILLA (0xc7)
		XK_Egrave,			// EGRAVE (0xc8)
		XK_Eacute,			// EACUTE (0xc9)
		XK_Ecircumflex,		// ECIRCUMFLEX (0xca)
		XK_Ediaeresis,		// EDIAERESIS (0xcb)
		XK_Igrave,			// IGRAVE (0xcc)
		XK_Iacute,			// IACUTE (0xcd)
		XK_Icircumflex,		// ICIRCUMFLEX (0xce)
		XK_Idiaeresis,		// IDIAERESIS (0xcf)
		XK_ETH,				// ETH (0xd0)
		XK_Ntilde,			// NTILDE (0xd1)
		XK_Ograve,			// OGRAVE (0xd2)
		XK_Oacute,			// OACUTE (0xd3)
		XK_Ocircumflex,		// OCIRCUMFLEX (0xd4)
		XK_Otilde,			// OTILDE (0xd5)
		XK_Odiaeresis,		// ODIAERESIS (0xd6)
		XK_multiply,		// MULTIPLY (0xd7)
		XK_Oslash,			// OSLASH (0xd8)
		XK_Ugrave,			// UGRAVE (0xd9)
		XK_Uacute,			// UACUTE (0xda)
		XK_Ucircumflex,		// UCIRCUMFLEX (0xdb)
		XK_Udiaeresis,		// UDIAERESIS (0xdc)
		XK_Yacute,			// YACUTE (0xdd)
		XK_THORN,			// THORN (0xde)
		XK_ssharp,			// SSHARP (0xdf)
		XK_agrave,			// aGRAVE (0xe0)
		XK_aacute,			// aACUTE (0xe1)
		XK_acircumflex,		// aCIRCUMFLEX (0xe2)
		XK_atilde,			// aTILDE (0xe3)
		XK_adiaeresis,		// aDIAERESIS (0xe4)
		XK_aring,			// aRING (0xe5)
		XK_ae,				// ae (0xe6)
		XK_ccedilla,		// cCEDILLA (0xe7)
		XK_egrave,			// eGRAVE (0xe8)
		XK_eacute,			// eACUTE (0xe9)
		XK_ecircumflex,		// eCIRCUMFLEX (0xea)
		XK_ediaeresis,		// eDIAERESIS (0xeb)
		XK_igrave,			// iGRAVE (0xec)
		XK_iacute,			// iACUTE (0xed)
		XK_icircumflex,		// iCIRCUMFLEX (0xee)
		XK_idiaeresis,		// iDIAERESIS (0xef)
		XK_eth,				// eth (0xf0)
		XK_ntilde,			// nTILDE (0xf1)
		XK_ograve,			// oGRAVE (0xf2)
		XK_oacute,			// oACUTE (0xf3)
		XK_ocircumflex,		// oCIRCUMFLEX (0xf4)
		XK_otilde,			// oTILDE (0xf5)
		XK_odiaeresis,		// oDIAERESIS (0xf6)
		XK_division,		// DIVISION (0xf7)
		XK_oslash,			// oSLASH (0xf8)
		XK_ugrave,			// uGRAVE (0xf9)
		XK_uacute,			// uACUTE (0xfa)
		XK_ucircumflex,		// uCIRCUMFLEX (0xfb)
		XK_udiaeresis,		// uDIAERESIS (0xfc)
		XK_yacute,			// yACUTE (0xfd)
		XK_thorn,			// thorn (0xfe)
		XK_ydiaeresis,		// yDIAERESIS (0xff)
		
		XK_KP_Insert,		// KEYPAD_0
		XK_KP_End,			// KEYPAD_1
		XK_KP_Down,			// KEYPAD_2
		XK_KP_Page_Down,	// KEYPAD_3
		XK_KP_Left,			// KEYPAD_4
		XK_KP_Begin,		// KEYPAD_5
		XK_KP_Right,		// KEYPAD_6
		XK_KP_Home,			// KEYPAD_7
		XK_KP_Up,			// KEYPAD_8
		XK_KP_Page_Up,		// KEYPAD_9
		XK_KP_Delete, 	// KEYPAD_SEPARATOR
		XK_KP_Divide,	 	// KEYPAD_DIVIDE
		XK_KP_Multiply, 	// KEYPAD_MULTIPLY
		XK_KP_Add, 			// KEYPAD_PLUS
		XK_KP_Subtract, 	// KEYPAD_MINUS
		XK_KP_Enter,		// KEYPAD_ENTER
		
		0 // terminator
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

