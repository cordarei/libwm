#ifndef PIXELFORMAT_IMPL_HPP
#define PIXELFORMAT_IMPL_HPP

#include <wm/pixelformat.hpp>

#include <GL/glx.h>
#include <X11/Xlib.h>

namespace wm
{
	struct PixelFormat::impl_t
	{
	
		XVisualInfo *visualinfo;
#ifdef GLX_VERSION_1_3
		GLXFBConfig fbconfig;
#endif

		Visual *visual;
		int depth;		
	};
}

#endif

