#ifndef WINDOW_IMPL_HPP
#define WINDOW_IMPL_HPP

#include <windows.h>

#include <wm/window.hpp>
#include <wm/pixelformat.hpp>

namespace wm
{
	struct Window::impl_t
	{
		explicit impl_t(const PixelFormat &format = PixelFormat())
			: format(format)
		{
		}

		PixelFormat format;
		HWND hwnd;
	};
}

#endif