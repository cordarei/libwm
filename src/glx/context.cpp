#include <memory>
#include <iostream>

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

namespace
{
	void makeCurrent(Display *xdisplay, const wm::glx::Extensions &ext, GLXContext ctx, GLXDrawable draw, GLXDrawable read)
	{
#ifdef GLX_VERSION_1_3
		if(ext.supported(1, 3))
		{
			if(ext.glXMakeContextCurrent(xdisplay, draw, read, ctx) != True)
				throw wm::Exception("Can't set current OpenGL context");
		} else
#endif
		{
			if(read != draw)
				throw wm::Exception("Separate draw and read buffers not supported");

			if(glXMakeCurrent(xdisplay, draw, ctx) != True)
				throw wm::Exception("Can't set current OpenGL context");
		}
	}

	void getCurrent(const wm::glx::Extensions &ext, Display *&xdisplay, GLXContext &ctx, GLXDrawable &draw, GLXDrawable &read)
	{
		xdisplay = glXGetCurrentDisplay();
		draw = glXGetCurrentDrawable();
		ctx = glXGetCurrentContext();        

#ifdef GLX_VERSION_1_3
		if(ext.supported(1, 3))
		{
			read = ext.glXGetCurrentReadDrawable();
		} else
#endif
		{
			read = draw;
		}
	}
}

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
		: impl(new impl_t(format.configuration()))
		, display_(&format.configuration().display())
	{
		std::auto_ptr<impl_t> impl_guard(impl); // deletes impl object in case of exception
		const glx::Extensions& extensions = impl->config.impl->extensions;
	
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
	
	CurrentContext::CurrentContext(Context &context, Surface &draw, Surface &read, bool do_setup)
		: context(context)
		, draw(draw)
		, read(read)
		, do_reset(false)
		, num1(0), num2(0), num3(0)
		, ptr1(0), ptr2(0), ptr3(0)
	{
		if(do_setup) setup();
	}

	CurrentContext::CurrentContext(Context &context, Surface &drawread, bool do_setup)
		: context(context)
		, draw(drawread)
		, read(drawread)
		, do_reset(false)
		, num1(0), num2(0), num3(0)
		, ptr1(0), ptr2(0), ptr3(0)
	{
		if(do_setup) setup();
	}

	CurrentContext::~CurrentContext()
	{
		try
		{
			if(do_reset) reset();
		} catch(wm::Exception& exc)
		{
			std::cerr << exc.what() << std::endl;
		}
	}

	void CurrentContext::setup()
	{
		GLXContext ctx;
		GLXDrawable dr, rd;
		::Display *olddpy;

		const glx::Extensions& ext = context.impl->config.impl->extensions;
		getCurrent(ext, olddpy, ctx, dr, rd);        

		::Display *xdisplay = draw.impl->xdisplay;
		makeCurrent(xdisplay, ext, context.impl->context, draw.impl->glxwindow, read.impl->glxwindow);

		if(olddpy != xdisplay ||
			ctx != context.impl->context ||
			dr != draw.impl->glxwindow ||
			rd != read.impl->glxwindow)
		{
			ptr1 = ctx;
			ptr2 = olddpy;
			num1 = dr;
			num2 = rd;
		}

		do_reset = true;
	}

	void CurrentContext::reset()
	{
		::Display *xdisplay = draw.impl->xdisplay;
		const glx::Extensions& ext = context.impl->config.impl->extensions;

		typedef ::Display* XDisplayPtr; // avoids a ton of strange error messages from gcc
		::Display *olddisplay = reinterpret_cast<XDisplayPtr>(ptr2);
		makeCurrent(olddisplay ? olddisplay : xdisplay,
			ext,
			reinterpret_cast<GLXContext>(ptr1), num1, num2);
		do_reset = false;
	}

	void CurrentContext::release()
	{
		do_reset = false;
	}

	void CurrentContext::swapInterval(Configuration& config, int interval)
	{
		const glx::Extensions &ext = config.impl->extensions;
		
		if(!ext.SGI_swap_control)
			throw Exception("Swap control not supported");
			
		if(ext.glXSwapIntervalSGI(interval) != 0)
			throw Exception("Setting swap interval failed");
	}
	
	void CurrentContext::swapInterval(int interval)
	{
		swapInterval(context.impl->config, interval);
	}
}

