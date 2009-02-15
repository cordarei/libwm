#ifndef WINDOW_IMPL_HPP
#define WINDOW_IMPL_HPP

#include <windows.h>

#include <wm/window.hpp>
#include <wm/pixelformat.hpp>

namespace wm
{
	struct Window::impl_t
	{
		HWND hwnd;
	};
}

#endif