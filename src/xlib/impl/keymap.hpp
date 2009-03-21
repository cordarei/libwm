#ifndef WM_XLIB_IMPL_KEYMAP_HPP
#define WM_XLIB_IMPL_KEYMAP_HPP

#include <X11/Xlib.h>

#include <wm/events/keyevent.hpp>

namespace wm
{
	namespace xlib
	{
		wm::KeyEvent::Symbol mapXKeySym(KeySym sym);
	}
}

#endif


