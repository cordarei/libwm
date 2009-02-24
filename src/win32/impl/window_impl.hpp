#ifndef WINDOW_IMPL_HPP
#define WINDOW_IMPL_HPP

#include <windows.h>

#include <wm/window.hpp>

#include <common/dispatcher.hpp>

namespace wm
{
	struct Window::impl_t
	{
		HWND hwnd;

		common::Dispatcher dispatcher;
	};
}

#endif
