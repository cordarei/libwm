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
		impl_t()
			: sizemove(false)
			, resizing(false)
			, dirty(false)
		{
		}

		// Window procedure state
		bool sizemove, resizing, dirty;
		unsigned int width, height;

		// Window handle
		HWND hwnd;

		// Event dispatching
		common::Dispatcher dispatcher;
		common::EventQueue eventq;
	};
}

#endif
