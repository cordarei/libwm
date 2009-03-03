#ifndef WINDOW_IMPL_HPP
#define WINDOW_IMPL_HPP

#include <X11/Xlib.h>
#include <GL/glx.h>

#include <wm/window.hpp>

#include <common/dispatcher.hpp>
#include <common/eventqueue.hpp>

namespace wm
{
	struct Window::impl_t
	{
		::Window window;
		XVisualInfo *visualinfo;
		
		common::Dispatcher dispatcher;
		common::EventQueue eventq;
	};
}

#endif

