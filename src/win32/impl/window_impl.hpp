#ifndef WINDOW_IMPL_HPP
#define WINDOW_IMPL_HPP

#include <windows.h>

#include <wm/window.hpp>

#include <common/dispatcher.hpp>
#include <common/eventqueue.hpp>

#include <win32/impl/eventreader.hpp>

namespace wm
{
	struct Window::impl_t
	{
		impl_t()
			: hwnd(0)
			, style(0), exstyle(0)
			, cursorVisible(true)
			, fullscreen(false)
			, minW(0), minH(0)
			, maxW(0), maxH(0)
		{
		}

		// Window handle
		HWND hwnd;

		// Window style and extended style
		int style, exstyle;

		// Is the mouse cursor visible in this Window?
		bool cursorVisible;

		// Is the window fullscreen
		bool fullscreen;

		// Window size limits
		unsigned int minW, minH, maxW, maxH;

		// Event dispatching
		common::Dispatcher dispatcher;
		common::EventQueue eventq;
		EventReader eventreader;
	};

	class PixelFormat;
	namespace win32
	{
		// NOTE: implemented in the wgl parts
		void setPixelFormat(HWND hwnd, const PixelFormat& format);
	};
}

#endif
