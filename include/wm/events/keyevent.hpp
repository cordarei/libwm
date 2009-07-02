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
				
				// 0x80 - 0x9f unused (total: 32)
				
				// International unicode characters 0xa0 - 0xff
				/// no-break space
				NOBREAKSPACE = 0xa0,
				/// inverted exclamation mark
				EXCLAMDOWN = 0xa1,
				/// cent sign
				CENT = 0xa2,
				/// pound sign
				STERLING = 0xa3,
				/// currency sign
				CURRENCY = 0xa4,
				/// yen sign
				YEN = 0xa5,
				/// broken bar
				BROKENBAR = 0xa6,
				/// section sign
				SECTION = 0xa7,
				/// diaeresis
				DIAERESIS = 0xa8,
				/// copyright sign
				COPYRIGHT = 0xa9,
				/// feminine ordinal indicator
				ORDFEMININE = 0xaa,
				/// left-pointing double angle quotation mark
				GUILLEMOTLEFT = 0xab,
				/// not sign
				NOTSIGN = 0xac,
				/// soft hyphen
				HYPHEN = 0xad,
				/// registered sign
				REGISTERED = 0xae,
				/// macron
				MACRON = 0xaf,
				/// degree sign
				DEGREE = 0xb0,
				/// plus-minus sign
				PLUSMINUS = 0xb1,
				/// superscript two
				TWOSUPERIOR = 0xb2,
				/// superscript three
				THREESUPERIOR = 0xb3,
				/// acute accent
				ACUTE = 0xb4,
				/// micro sign
				MU = 0xb5,
				/// pilcrow sign
				PARAGRAPH = 0xb6,
				/// middle dot
				PERIODCENTERED = 0xb7,
				/// cedilla
				CEDILLA = 0xb8,
				/// superscript one
				ONESUPERIOR = 0xb9,
				/// masculine ordinal indicator
				MASCULINE = 0xba,
				/// right-pointing double angle quotation mark
				GUILLEMOTRIGHT = 0xbb,
				/// vulgar fraction one quarter
				ONEQUARTER = 0xbc,
				/// vulgar fraction one half
				ONEHALF = 0xbd,
				/// vulgar fraction three quarters
				THREEQUARTERS = 0xbe,
				/// inverted question mark
				QUESTIONDOWN = 0xbf,
				/// latin capital letter a with grave
				AGRAVE = 0xc0,
				/// latin capital letter a with acute
				AACUTE = 0xc1,
				/// latin capital letter a with circumflex
				ACIRCUMFLEX = 0xc2,
				/// latin capital letter a with tilde
				ATILDE = 0xc3,
				/// latin capital letter a with diaeresis
				ADIAERESIS = 0xc4,
				/// latin capital letter a with ring above
				ARING = 0xc5,
				/// latin capital letter ae
				AE = 0xc6,
				/// latin capital letter c with cedilla
				CCEDILLA = 0xc7,
				/// latin capital letter e with grave
				EGRAVE = 0xc8,
				/// latin capital letter e with acute
				EACUTE = 0xc9,
				/// latin capital letter e with circumflex
				ECIRCUMFLEX = 0xca,
				/// latin capital letter e with diaeresis
				EDIAERESIS = 0xcb,
				/// latin capital letter i with grave
				IGRAVE = 0xcc,
				/// latin capital letter i with acute
				IACUTE = 0xcd,
				/// latin capital letter i with circumflex
				ICIRCUMFLEX = 0xce,
				/// latin capital letter i with diaeresis
				IDIAERESIS = 0xcf,
				/// latin capital letter eth
				ETH = 0xd0,
				/// latin capital letter n with tilde
				NTILDE = 0xd1,
				/// latin capital letter o with grave
				OGRAVE = 0xd2,
				/// latin capital letter o with acute
				OACUTE = 0xd3,
				/// latin capital letter o with circumflex
				OCIRCUMFLEX = 0xd4,
				/// latin capital letter o with tilde
				OTILDE = 0xd5,
				/// latin capital letter o with diaeresis
				ODIAERESIS = 0xd6,
				/// multiplication sign
				MULTIPLY = 0xd7,
				/// latin capital letter o with stroke
				OSLASH = 0xd8,
				/// latin capital letter u with grave
				UGRAVE = 0xd9,
				/// latin capital letter u with acute
				UACUTE = 0xda,
				/// latin capital letter u with circumflex
				UCIRCUMFLEX = 0xdb,
				/// latin capital letter u with diaeresis
				UDIAERESIS = 0xdc,
				/// latin capital letter y with acute
				YACUTE = 0xdd,
				/// latin capital letter thorn
				THORN = 0xde,
				/// latin small letter sharp s
				SSHARP = 0xdf,
				/// latin small letter a with grave
				aGRAVE = 0xe0,
				/// latin small letter a with acute
				aACUTE = 0xe1,
				/// latin small letter a with circumflex
				aCIRCUMFLEX = 0xe2,
				/// latin small letter a with tilde
				aTILDE = 0xe3,
				/// latin small letter a with diaeresis
				aDIAERESIS = 0xe4,
				/// latin small letter a with ring above
				aRING = 0xe5,
				/// latin small letter ae
				ae = 0xe6,
				/// latin small letter c with cedilla
				cCEDILLA = 0xe7,
				/// latin small letter e with grave
				eGRAVE = 0xe8,
				/// latin small letter e with acute
				eACUTE = 0xe9,
				/// latin small letter e with circumflex
				eCIRCUMFLEX = 0xea,
				/// latin small letter e with diaeresis
				eDIAERESIS = 0xeb,
				/// latin small letter i with grave
				iGRAVE = 0xec,
				/// latin small letter i with acute
				iACUTE = 0xed,
				/// latin small letter i with circumflex
				iCIRCUMFLEX = 0xee,
				/// latin small letter i with diaeresis
				iDIAERESIS = 0xef,
				/// latin small letter eth
				eth = 0xf0,
				/// latin small letter n with tilde
				nTILDE = 0xf1,
				/// latin small letter o with grave
				oGRAVE = 0xf2,
				/// latin small letter o with acute
				oACUTE = 0xf3,
				/// latin small letter o with circumflex
				oCIRCUMFLEX = 0xf4,
				/// latin small letter o with tilde
				oTILDE = 0xf5,
				/// latin small letter o with diaeresis
				oDIAERESIS = 0xf6,
				/// division sign
				DIVISION = 0xf7,
				/// latin small letter o with stroke
				oSLASH = 0xf8,
				/// latin small letter u with grave
				uGRAVE = 0xf9,
				/// latin small letter u with acute
				uACUTE = 0xfa,
				/// latin small letter u with circumflex
				uCIRCUMFLEX = 0xfb,
				/// latin small letter u with diaeresis
				uDIAERESIS = 0xfc,
				/// latin small letter y with acute
				yACUTE = 0xfd,
				/// latin small letter thorn
				thorn = 0xfe,
				/// latin small letter y with diaeresis
				yDIAERESIS = 0xff,				

				KEYPAD_0 = 0x100,
				KEYPAD_1 = 0x101,
				KEYPAD_2 = 0x102,
				KEYPAD_3 = 0x103,
				KEYPAD_4 = 0x104,
				KEYPAD_5 = 0x105,
				KEYPAD_6 = 0x106,
				KEYPAD_7 = 0x107,
				KEYPAD_8 = 0x108,
				KEYPAD_9 = 0x109,
				KEYPAD_SEPARATOR = 0x10a,
				KEYPAD_DIVIDE = 0x10b,
				KEYPAD_MULTIPLY = 0x10c,
				KEYPAD_PLUS = 0x10d,
				KEYPAD_MINUS = 0x10e,
				KEYPAD_ENTER = 0x10f,

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
