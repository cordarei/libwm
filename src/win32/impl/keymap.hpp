#ifndef WM_WIN32_IMPL_KEYMAP_HPP
#define WM_WIN32_IMPL_KEYMAP_HPP

#include <wm/events/keyevent.hpp>

#include <windows.h>

namespace wm
{
	namespace win32
	{
		wm::KeyEvent::Symbol mapVirtualKeyCode(WPARAM vCode);
	}
}

#endif
