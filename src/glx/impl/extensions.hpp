#ifndef GLX_IMPL_EXTENSIONS_HPP
#define GLX_IMPL_EXTENSIONS_HPP

#include <X11/Xlib.h>
#include <GL/glx.h>

namespace wm
{
	namespace glx
	{
		struct Extensions
		{
			Extensions();
			~Extensions();
			void init(::Display *xdisplay, int screen);
			
			bool supported(int major, int minor) const;
		
			void *library;
			int versionMajor, versionMinor;

#ifdef GLX_VERSION_1_3
		GLXContext (*glXCreateNewContext)(::Display *dpy, GLXFBConfig config, int render_type, GLXContext share_list, Bool direct);

		GLXWindow (*glXCreateWindow)(::Display *dpy, GLXFBConfig config, ::Window win, const int *attrib_list);
		void (*glXDestroyWindow)(::Display *dpy, GLXWindow win);

		GLXFBConfig* (*glXGetFBConfigs)(::Display *dpy, int screen, int *nelements);
		int (*glXGetFBConfigAttrib)(::Display *dpy, GLXFBConfig config, int attribute, int *value);
		XVisualInfo* (*glXGetVisualFromFBConfig)(::Display *dpy, GLXFBConfig config);

		Bool (*glXMakeContextCurrent)(::Display *display, GLXDrawable draw, GLXDrawable read, GLXContext ctx);
#endif
		};
	}
}

#endif

