#ifndef SURFACE_IMPL_HPP
#define SURFACE_IMPL_HPP

#include <wm/surface.hpp>

#include <X11/Xlib.h>
#include <GL/glx.h>

namespace wm
{
	class Window;

	namespace glx
	{
		struct Extensions;
	}
	
	struct Surface::impl_t
	{
		::Display* xdisplay;
		
		glx::Extensions *extensions;
		
#ifdef GLX_VERSION_1_3
		GLXWindow glxwindow;
#endif
	};
}

#endif

