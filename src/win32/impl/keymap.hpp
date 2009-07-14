#ifndef WM_WIN32_IMPL_KEYMAP_HPP
#define WM_WIN32_IMPL_KEYMAP_HPP

#include <wm/keyboard.hpp>

#include <windows.h>

namespace wm
{
	namespace win32
	{
		wm::keyboard::Symbol mapVirtualKeyCode(WPARAM vCode);

		// NOTE: translateKeyEvent must be called from the Window Procedure
		wm::keyboard::Symbol translateKeyEvent(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam, bool &filter);

		wm::keyboard::KeyMod getKeyModState();
	}
}

#endif
