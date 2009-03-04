#ifndef WINDOW_IMPL_HPP
#define WINDOW_IMPL_HPP

#include <windows.h>

#include <wm/window.hpp>

#include <common/dispatcher.hpp>
#include <common/eventqueue.hpp>

namespace wm
{
	struct Window::impl_t
	{
		HWND hwnd;

		common::Dispatcher dispatcher;
		common::EventQueue eventq;
	};
}

#endif
