#include <memory>

#include <GL/glx.h>

#include <wm/exception.hpp>
#include <wm/display.hpp>
#include <wm/window.hpp>
#include <wm/context.hpp>
#include <wm/surface.hpp>

#include <glx/impl/context_impl.hpp>
#include <glx/impl/pixelformat_impl.hpp>
#include <glx/impl/configuration_impl.hpp>
#include <glx/impl/surface_impl.hpp>

#include <xlib/impl/display_impl.hpp>
#include <xlib/impl/window_impl.hpp>

#include <wm/export.hpp>

namespace wm
{
	Context::Context(
		const PixelFormat &format,
		int versionMajor,
		int versionMinor,
		bool compatible,
		bool debug,
		bool direct,
		Context *shared)
		: impl(new impl_t)
		, display_(&format.configuration().display())
	{
		std::auto_ptr<impl_t> impl_guard(impl); // deletes impl object in case of exception
		const glx::Extensions& extensions = format.configuration().impl->extensions;
		impl->extensions = &extensions;
	
		::Display *xdisplay = display().impl->display;

		impl->context = 0;
#ifdef GLX_VERSION_1_3
		int render_type = GLX_RGBA_TYPE;
		if(format.descriptor().type == PixelFormat::FLOAT) render_type = GLX_RGBA_FLOAT_TYPE_ARB;
		if(format.descriptor().type == PixelFormat::UNSIGNED_FLOAT) render_type = GLX_RGBA_UNSIGNED_FLOAT_TYPE_EXT;

		if(extensions.ARB_create_context)
		{
			// NOTE: GLX_CONTEXT_PROFILE_MASK_ARB is valid only with OpenGL version >= 3.2
			bool useProfile = extensions.ARB_create_context_profile &&
				(versionMajor > 3 || (versionMajor == 3 && versionMinor >= 2));
		
			int attribs[] = {
				GLX_RENDER_TYPE, render_type,
				GLX_CONTEXT_MAJOR_VERSION_ARB, versionMajor,
				GLX_CONTEXT_MINOR_VERSION_ARB, versionMinor,
				GLX_CONTEXT_FLAGS_ARB, 0
					| (debug ? GLX_CONTEXT_DEBUG_BIT_ARB : 0)
					| (compatible ? 0 : GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB),
				(useProfile ? GLX_CONTEXT_PROFILE_MASK_ARB : 0),
					(compatible ?
						GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB :
						GLX_CONTEXT_CORE_PROFILE_BIT_ARB),
				0 };
		
			impl->context = extensions.glXCreateContextAttribsARB(
				xdisplay,
				format.impl->fbconfig,
				shared ? shared->impl->context : 0,
				direct,
				attribs
				);
		} else if(extensions.supported(1, 3))
		{
			impl->context = 
				extensions.glXCreateNewContext(
					xdisplay,
					format.impl->fbconfig,
					render_type,
					shared ? shared->impl->context : 0,
					direct
					);
		} else
#endif
		{
			impl->context = glXCreateContext(
				xdisplay,
				format.impl->visualinfo, 
				shared ? shared->impl->context : 0,
				direct);
		}
				
		if(!impl->context)
		{
			throw wm::Exception("Can't create OpenGL context");
		}

		
		impl_guard.release();
	}

	Context::~Context()
	{
		glXDestroyContext(
			display().impl->display,
			impl->context);
			
		delete impl;
	}
	
	bool Context::direct() const
	{
		::Display *xdisplay = display().impl->display;
		return glXIsDirect(xdisplay, impl->context);
	}
	
	void WM_EXPORT makeCurrent(Context &context, Surface &draw, Surface &read)
	{
		::Display* xdisplay = context.display().impl->display;

#ifdef GLX_VERSION_1_3
		if(context.impl->extensions->supported(1, 3))
		{
			if(context.impl->extensions->glXMakeContextCurrent(
				xdisplay, draw.impl->glxwindow, read.impl->glxwindow, context.impl->context)
					!= True)
			{
				throw wm::Exception("Can't set current OpenGL context");
			}
		} else
#endif
		{
			if(&read != &draw)
				throw wm::Exception("Separate draw and read buffers not supported");

			if(glXMakeCurrent(xdisplay, draw.window().impl->window, context.impl->context)
				!= True)
			{
				throw wm::Exception("Can't set current OpenGL context");
			}
		}
	}
}

