#ifndef WM_EVENTS_KEYEVENT_HPP
#define WM_EVENTS_KEYEVENT_HPP

#include <wm/event.hpp>
#include <wm/eventhandler.hpp>

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
			/// Key symbol enumeration
			/**
			
				@note keys representing the Latin-1 supplement characters
					(Unicode 0xa0-0xff) are usually mapped to lower case
					charaters, for example aDIARESIS instead of ADIARESIS.
				@warning the numeric values of the enumerants may change, use symbols instead
			*/
			enum Symbol {
				/// Unknown key
				UNKNOWN = 0,
				
				// 0x01 - 0x07 unused (total: 7)
				
				
				/// Backspace
				BACKSPACE = 0x08,
				/// Tabulator
				TAB = 0x09,
				/// Line feed
				LINEFEED = 0x0a,
				/// Clear
				CLEAR = 0x0b,
				
				// 0x0c unused
				
				/// Return/Enter key
				RETURN = 0x0d,
				
				// 0x0e - 0x1a unused (total: 13)
				
				/// The Escape key
				ESCAPE = 0x1b,
				
				// 0x1c - 0x1f unused (total: 4)
				
				/// The space bar
				SPACE = 0x20,
				/// Exclamation sign
				EXCLAIM		= 0x21,
				/// Double quotes
				DOUBLEQUOTE = 0x22,
				/// Hash sign (#)
				HASH		= 0x23,
				/// Dollar sign
				DOLLAR		= 0x24,
				
				// 0x25 unused
				
				/// Ampersand
				AMPERSAND		= 0x26,
				/// Quote
				QUOTE		= 0x27,
				/// Left parenthesis
				LEFTPAREN		= 0x28,
				/// Right parenthesis
				RIGHTPAREN		= 0x29,
				/// Asterisk
				ASTERISK		= 0x2a,
				/// Plus
				PLUS		= 0x2b,
				/// Comma
				COMMA		= 0x2c,
				/// Minus
				MINUS		= 0x2d,
				/// Period
				PERIOD		= 0x2e,
				/// Slash
				SLASH		= 0x2f,

				/// Number 0
				KEY_0 = 0x30,
				/// Number 1
				KEY_1 = 0x31,
				/// Number 2
				KEY_2 = 0x32,
				/// Number 3
				KEY_3 = 0x33,
				/// Number 4
				KEY_4 = 0x34,
				/// Number 5
				KEY_5 = 0x35,
				/// Number 6
				KEY_6 = 0x36,
				/// Number 7
				KEY_7 = 0x37,
				/// Number 8
				KEY_8 = 0x38,
				/// Number 9
				KEY_9 = 0x39,
				
				/// Colon
				COLON = 0x3a,
				/// Semicolon
				SEMICOLON = 0x3b,
				/// Less than
				LESS = 0x3c,
				/// Equals
				EQUALS = 0x3d,
				/// Greater than
				GREATER = 0x3e,
				/// Question mark
				QUESTION = 0x3f,
				/// At sign (@)
				AT = 0x40,
				
				/// Key 'A'
				KEY_A = 0x41,
				/// Key 'B'
				KEY_B = 0x42,
				/// Key 'C'
				KEY_C = 0x43,
				/// Key 'D'
				KEY_D = 0x44,
				/// Key 'E'
				KEY_E = 0x45,
				/// Key 'F'
				KEY_F = 0x46,
				/// Key 'G'
				KEY_G = 0x47,
				/// Key 'H'
				KEY_H = 0x48,
				/// Key 'I'
				KEY_I = 0x49,
				/// Key 'J'
				KEY_J = 0x4a,
				/// Key 'K'
				KEY_K = 0x4b,
				/// Key 'L'
				KEY_L = 0x4c,
				/// Key 'M'
				KEY_M = 0x4d,
				/// Key 'N'
				KEY_N = 0x4e,
				/// Key 'O'
				KEY_O = 0x4f,
				/// Key 'P'
				KEY_P = 0x50,
				/// Key 'Q'
				KEY_Q = 0x51,
				/// Key 'R'
				KEY_R = 0x52,
				/// Key 'S'
				KEY_S = 0x53,
				/// Key 'T'
				KEY_T = 0x54,
				/// Key 'U'
				KEY_U = 0x55,
				/// Key 'V'
				KEY_V = 0x56,
				/// Key 'W'
				KEY_W = 0x57,
				/// Key 'X'
				KEY_X = 0x58,
				/// Key 'Y'
				KEY_Y = 0x59,
				/// Key 'Z'
				KEY_Z = 0x5a,

				/// Left bracket
				LEFTBRACKET = 0x5b,
				/// Backslash
				BACKSLASH = 0x5c,
				/// Right bracket
				RIGHTBRACKET = 0x5d,
				/// Caret
				CARET = 0x5e,
				/// Underscore
				UNDERSCORE = 0x5f,
				/// Backquote
				BACKQUOTE = 0x60,
				
				// 0x61 - 0x7e unused (total: 30)
				
				/// Delete
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

				/// Keypad 0
				KEYPAD_0 = 0x100,
				/// Keypad 1
				KEYPAD_1 = 0x101,
				/// Keypad 2
				KEYPAD_2 = 0x102,
				/// Keypad 3
				KEYPAD_3 = 0x103,
				/// Keypad 4
				KEYPAD_4 = 0x104,
				/// Keypad 5
				KEYPAD_5 = 0x105,
				/// Keypad 6
				KEYPAD_6 = 0x106,
				/// Keypad 7
				KEYPAD_7 = 0x107,
				/// Keypad 8
				KEYPAD_8 = 0x108,
				/// Keypad 9
				KEYPAD_9 = 0x109,
				/// Keypad comma
				KEYPAD_SEPARATOR = 0x10a,
				/// Keypad divide
				KEYPAD_DIVIDE = 0x10b,
				/// Keypad multiply
				KEYPAD_MULTIPLY = 0x10c,
				/// Keypad plus
				KEYPAD_PLUS = 0x10d,
				/// Keypad minus
				KEYPAD_MINUS = 0x10e,
				/// Keypad enter
				KEYPAD_ENTER = 0x10f,
				
				/// The up arrow
				UP = 0x110,
				/// The down arrow
				DOWN = 0x111,
				/// The left arrow
				LEFT = 0x112,
				/// The right arrow
				RIGHT = 0x113,
				///	Insert
				INSERT = 0x114,
				/// Home
				HOME = 0x115,
				/// End
				END = 0x116,
				/// Page up
				PAGEUP = 0x117,
				/// Page down
				PAGEDOWN = 0x118,
				
				/// Function key F1
				F1 = 0x119,
				/// Function key F2
				F2 = 0x11a,
				/// Function key F3
				F3 = 0x11b,
				/// Function key F4
				F4 = 0x11c,
				/// Function key F5
				F5 = 0x11d,
				/// Function key F6
				F6 = 0x11e,
				/// Function key F7
				F7 = 0x11f,
				/// Function key F8
				F8 = 0x120,
				/// Function key F9
				F9 = 0x121,
				/// Function key F10
				F10 = 0x122,
				/// Function key F11
				F11 = 0x123,
				/// Function key F12
				F12 = 0x124,
				/// Function key F13
				F13 = 0x125,
				/// Function key F14
				F14 = 0x126,
				/// Function key F15
				F15 = 0x127,
				/// Function key F16
				F16 = 0x128,
				/// Function key F17
				F17 = 0x129,
				/// Function key F18
				F18 = 0x12a,
				/// Function key F19
				F19 = 0x12b,
				/// Function key F20
				F20 = 0x12c,
				/// Function key F21
				F21 = 0x12d,
				/// Function key F22
				F22 = 0x12e,
				/// Function key F23
				F23 = 0x12f,
				/// Function key F24
				F24 = 0x130,
				
				/// Num Lock
				NUMLOCK = 0x131,
				/// Caps Lock
				CAPSLOCK = 0x132,
				/// Scroll Lock
				SCROLLLOCK = 0x133,
				
				/// Left shift
				LSHIFT = 0x134,
				/// Right shift
				RSHIFT = 0x135,
				/// Left control
				LCTRL = 0x136,
				/// Right control
				RCRTL = 0x137,
				/// Left ALT
				LALT = 0x138,
				/// Right ALT
				RALT = 0x139,
				/// Left Super key
				LSUPER = 0x140,
				/// Right Super key
				RSUPER = 0x141,

				/// AltGR
				ALTGR = 0x142,
				/// Multi key compose
				COMPOSE = 0x143,
				/// Menu key (usually between right Super and Control)
				MENU = 0x144,
				
				/// System request
				SYS_REQ = 0x145,
				/// Pause
				PAUSE = 0x146,
				/// Print screen
				PRINT = 0x147,
				
				/// Dead grave accent
				DEAD_GRAVE = 0x148,
				/// Dead acute accent
				DEAD_ACUTE = 0x149,
				/// Dead circumflex
				DEAD_CIRCUMFLEX = 0x14a,
				/// Dead tilde
				DEAD_TILDE = 0x14b,
				/// Dead macron
				DEAD_MACRON = 0x14c,
				/// Dead breve
				DEAD_BREVE = 0x14d,
				/// Dead dot above letter
				DEAD_ABOVEDOT = 0x14e,
				/// Dead diaeresis
				DEAD_DIAERESIS = 0x14f,
				/// Dead ring above letter
				DEAD_ABOVERING = 0x150,
				
				NUM_KEYSYMS // TERMINATOR
				};
		
			/// Create a new key event
			/**
				@param window the Window object that recieved the key event
				@param symbol the key Symbol
				@param state true if the key was pressed, false if released
			*/
			KeyEvent(
				Window &window,
				Symbol symbol,
				bool state)
				: Event(window)
				, symbol_(symbol)
				, state_(state)
			{
			}
			
			/// The key symbol
			/**
				@return the key symbol of the pressed/released key
			*/
			Symbol symbol() const { return symbol_; }
			
			/// The state of the key
			/**
				@return true if the key was pressed, false if released			
			*/			
			bool state() const { return state_; }

			/// Visitor pattern entry point
			virtual void accept(EventHandler &handler) const { handler.handle(*this); }
			
		private:
			Symbol symbol_;
			bool state_;
	};
	
	/// Get a string representation of a key symbol (in UTF-8)
	/**
		Some key symbols representing international character symbols
		(most notably the Latin-1 supplement of unicode) are represented
		in UTF-8.
		
		@return a string representation of a key symbol	(UTF-8)
	*/
	const char WM_EXPORT *keyName(wm::KeyEvent::Symbol);
}

#undef WM_EXPORT

#endif
