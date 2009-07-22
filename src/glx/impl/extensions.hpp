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
			PFNGLXGETFBCONFIGSPROC glXGetFBConfigs;
			PFNGLXGETFBCONFIGATTRIBPROC glXGetFBConfigAttrib;
			PFNGLXGETVISUALFROMFBCONFIGPROC glXGetVisualFromFBConfig;
			
			PFNGLXCREATEWINDOWPROC glXCreateWindow;
			PFNGLXDESTROYWINDOWPROC glXDestroyWindow;
			
			PFNGLXCREATENEWCONTEXTPROC glXCreateNewContext;
			PFNGLXMAKECONTEXTCURRENTPROC glXMakeContextCurrent;
#endif
		};
	}
}

#endif

