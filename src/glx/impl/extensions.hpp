#ifndef GLX_IMPL_EXTENSIONS_HPP
#define GLX_IMPL_EXTENSIONS_HPP

#include <set>
#include <string>

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
			bool supported(const std::string &ext) const;

			typedef void (*GLXextFuncPtr)(void);

			GLXextFuncPtr getProcAddress(const GLubyte* name) const;
	
			template <typename T>
			void getProcAddress(const GLubyte* name, T* &funcptr) const
			{
				GLXextFuncPtr ptr = getProcAddress(name);
				funcptr = reinterpret_cast<T*>(ptr);
			}
			
			void *library;
			int versionMajor, versionMinor;
			
			std::set<std::string> extensions;

#ifdef GLX_VERSION_1_3
			GLXContext (*glXCreateNewContext)(::Display *dpy, GLXFBConfig config, int render_type, GLXContext share_list, Bool direct);

			GLXWindow (*glXCreateWindow)(::Display *dpy, GLXFBConfig config, ::Window win, const int *attrib_list);
			void (*glXDestroyWindow)(::Display *dpy, GLXWindow win);
			int (*glXQueryDrawable)(::Display *dpy, GLXDrawable draw, int attribute, unsigned int *value);

			GLXFBConfig* (*glXGetFBConfigs)(::Display *dpy, int screen, int *nelements);
			int (*glXGetFBConfigAttrib)(::Display *dpy, GLXFBConfig config, int attribute, int *value);
			XVisualInfo* (*glXGetVisualFromFBConfig)(::Display *dpy, GLXFBConfig config);

			Bool (*glXMakeContextCurrent)(::Display *display, GLXDrawable draw, GLXDrawable read, GLXContext ctx);
			GLXDrawable (*glXGetCurrentReadDrawable)();
#endif

			GLXextFuncPtr (*glXGetProcAddress)(const GLubyte*);
			
			bool ARB_get_proc_address;
			GLXextFuncPtr (*glXGetProcAddressARB)(const GLubyte*);
			
#ifdef GLX_VERSION_1_3
			bool ARB_create_context, ARB_create_context_profile;
			GLXContext (*glXCreateContextAttribsARB)(::Display *dpy, GLXFBConfig config, GLXContext share_context, Bool direct, const int *attrib_list);
#endif

			bool ARB_multisample;
			
			bool EXT_framebuffer_sRGB;
			bool ARB_framebuffer_sRGB;
			
			bool ARB_fbconfig_float;
			bool EXT_fbconfig_packed_float;
			
			bool EXT_visual_rating;
			
			int (*glXSwapIntervalSGI)(int interval);
			bool SGI_swap_control;
		};
	}
}

#ifndef GLX_ARB_create_context
#define GLX_CONTEXT_DEBUG_BIT_ARB          0x00000001
#define GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x00000002
#define GLX_CONTEXT_MAJOR_VERSION_ARB      0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB      0x2092
#define GLX_CONTEXT_FLAGS_ARB              0x2094
#endif

#ifndef GLX_ARB_create_context_profile
#define GLX_CONTEXT_PROFILE_MASK_ARB		0x9126
#define GLX_CONTEXT_CORE_PROFILE_BIT_ARB	0x00000001
#define GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002
#endif

#ifndef GLX_ARB_multisample
#define GLX_SAMPLE_BUFFERS_ARB				100000
#define GLX_SAMPLES_ARB						100001
#endif

#ifndef GLX_EXT_framebuffer_sRGB
#define GLX_FRAMEBUFFER_SRGB_CAPABLE_EXT	0x20B2
#endif

#ifndef GLX_ARB_framebuffer_sRGB
#define GLX_FRAMEBUFFER_SRGB_CAPABLE_ARB	0x20B2
#endif

#ifndef GLX_ARB_fbconfig_float
#define GLX_RGBA_FLOAT_TYPE_ARB				0x20B9
#define GLX_RGBA_FLOAT_BIT_ARB				0x00000004
#endif

#ifndef GLX_EXT_fbconfig_packed_float
#define GLX_RGBA_UNSIGNED_FLOAT_TYPE_EXT	0x20B1
#define GLX_RGBA_UNSIGNED_FLOAT_BIT_EXT		0x00000008
#endif

#ifndef GLX_EXT_visual_rating
#define GLX_VISUAL_CAVEAT_EXT				0x20
#define GLX_NONE_EXT						0x8000
#define GLX_SLOW_VISUAL_EXT					0x8001
#define GLX_NON_CONFORMANT_VISUAL_EXT		0x800D
#endif

#ifndef GLX_SAMPLES // workaround for broken glx.h/glxext.h
#define GLX_SAMPLES							100001
#endif
#ifndef GLX_SAMPLE_BUFFERS
#define GLX_SAMPLE_BUFFERS					100000
#endif

#endif

