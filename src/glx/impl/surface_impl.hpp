#ifndef SURFACE_IMPL_HPP
#define SURFACE_IMPL_HPP

#include <wm/surface.hpp>

#include <X11/Xlib.h>
#include <GL/glx.h>

namespace wm
{
	class Window;

	struct Surface::impl_t
	{
		::Display* xdisplay;
		GLXWindow glxwindow;
	};
}

#endif

