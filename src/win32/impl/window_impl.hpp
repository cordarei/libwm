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
		EventReader eventreader;

		// Window handle
		HWND hwnd;

		// Window style and extended style
		int style, exstyle;

		// Event dispatching
		common::Dispatcher dispatcher;
		common::EventQueue eventq;
	};
}

#endif
