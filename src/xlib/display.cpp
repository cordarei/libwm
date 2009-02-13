#include <X11/Xlib.h>

#include <wm/exception.hpp>
#include <wm/display.hpp>
#include "impl/display_impl.hpp"

namespace wm
{
	Display::Display(const char *name)
		: impl(new impl_t)
	{
		impl->display = XOpenDisplay(name);
		if(!impl->display) throw Exception("Can't open Display");
	}
	
	Display::~Display()
	{
		XCloseDisplay(impl->display);
	}
}

