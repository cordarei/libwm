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
	Context::Context(const PixelFormat &format, Context *shared)
		: impl(new impl_t)
		, display_(&format.configuration().display())
	{
		std::auto_ptr<impl_t> impl_guard(impl); // deletes impl object in case of exception
		impl->extensions = &format.configuration().impl->extensions;
	
		::Display *xdisplay = display().impl->display;

		impl->context = 0;
#ifdef GLX_VERSION_1_3
		if(impl->extensions->supported(1, 3))
		{
			impl->context = 
				impl->extensions->glXCreateNewContext(
					xdisplay,
					format.impl->fbconfig,
					GLX_RGBA_TYPE,
					shared ? shared->impl->context : 0,
					True
					);
		} else
#endif
		{
			impl->context = glXCreateContext(
				xdisplay,
				format.impl->visualinfo, 
				shared ? shared->impl->context : 0,
				True);
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

