#ifndef WINDOW_IMPL_HPP
#define WINDOW_IMPL_HPP

#include <X11/Xlib.h>
#include <GL/glx.h>

#include <wm/window.hpp>

#include <common/dispatcher.hpp>

namespace wm
{
	struct Window::impl_t
	{
		::Window window;
		XVisualInfo *visualinfo;
		long event_mask;
		
		common::Dispatcher dispatcher;
	};
}

#endif

