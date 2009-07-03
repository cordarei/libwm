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
		explicit impl_t(Window& window) : window(&window)
		{
		}
	
		Window *window;
	
		Colormap colormap;
	
		::Window subwindow;
		GLXWindow glxwindow;
	};
}

#endif

