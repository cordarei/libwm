#ifndef WINDOW_IMPL_HPP
#define WINDOW_IMPL_HPP

#include <X11/Xlib.h>
#include <GL/glx.h>

#include <wm/window.hpp>

namespace wm
{
	struct Window::impl_t
	{
		::Window window;
		XVisualInfo *visualinfo;
	};
}

#endif

