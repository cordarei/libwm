#ifndef WM_XLIB_IMPL_KEYMAP_HPP
#define WM_XLIB_IMPL_KEYMAP_HPP

#include <X11/Xlib.h>

#include <wm/keyboard.hpp>
#include <wm/mouse.hpp>

namespace wm
{
	namespace xlib
	{
		wm::keyboard::Symbol mapXKeySym(KeySym sym);
		wm::keyboard::KeyMod mapKeyMod(unsigned int state);

		wm::mouse::ButtonMask mapButtons(unsigned int state);
		wm::mouse::Button mapButton(unsigned int button);
	}
}

#endif


