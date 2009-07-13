#ifndef WM_MOUSE_HPP
#define WM_MOUSE_HPP

#include <wm/export.hpp>

namespace wm
{
	/// Mouse button enumerants and utility functions
	namespace mouse
	{
		/// Mouse button enumeration
		enum Button {
			/// Unknown mouse button
			UNKNOWN = 0,
			/// Left mouse button
			BUTTON_LEFT = 1, 
			/// Middle mouse button
			BUTTON_MIDDLE = 2,
			/// Right mouse button
			BUTTON_RIGHT = 3,
			/// Mouse wheel up
			WHEEL_UP = 4,
			/// Mouse wheel down
			WHEEL_DOWN = 5,
			/// Scroll left
			SCROLL_LEFT = 6,
			/// Scroll right
			SCROLL_RIGHT = 7,
			/// X Button
			BUTTON_X = 8,
			/// Y Button
			BUTTON_Y = 9
			};
		
		/// Get a string representation of a button
		const char WM_EXPORT *buttonName(Button button);
		
		/// Mouse button mask type
		typedef unsigned char ButtonMask;
		
		/// Left mouse button mask
		const ButtonMask MASK_LEFT = 1 << 0;
		/// Middle mouse button mask
		const ButtonMask MASK_MIDDLE = 1 << 1;
		/// Right mouse button mask
		const ButtonMask MASK_RIGHT = 1 << 2;
	}
}

#endif

