#ifndef DISPLAY_IMPL_HPP
#define DISPLAY_IMPL_HPP

#include <X11/Xlib.h>

#include <wm/display.hpp>

namespace wm
{
	struct Display::impl_t
	{
		::Display *display;
	};
}

#endif

