#include <GL/glx.h>

#include <wm/exception.hpp>
#include <wm/display.hpp>
#include <wm/window.hpp>
#include <wm/context.hpp>
#include "impl/display_impl.hpp"
#include "impl/window_impl.hpp"
#include "impl/context_impl.hpp"

namespace
{
	GLXContext createContext(
		Display* display,
		XVisualInfo *visualinfo,
		GLXContext shared
		)
	{
		GLXContext context = glXCreateContext(
			display,
			visualinfo,
			shared,
			1);
		if(!context)
			throw wm::Exception("Can't create Context");
			
		// TODO: better error handling
		return context;
	}
}

namespace wm
{
	Context::Context(Window &window, int majorVersion, int minorVersion)
		: impl(new impl_t)
		, display_(window.display())
	{
		(void)majorVersion;
		(void)minorVersion;
		
		impl->context = createContext(
			window.display().impl->display,
			window.impl->visualinfo,
			0);
	}
	
	Context::Context(Window &window, Context& sharedContext, int majorVersion, int minorVersion)
		: impl(new impl_t)
		, display_(window.display())
	{
		(void)majorVersion;
		(void)minorVersion;

		impl->context = createContext(
			window.display().impl->display,
			window.impl->visualinfo,
			sharedContext.impl->context);
	}
	
	Context::~Context()
	{
		glXDestroyContext(
			display().impl->display,
			impl->context);
			
		delete impl;
	}
	
	void makeCurrent(Window& window, Context& context)
	{
		if(!glXMakeCurrent(
			window.display().impl->display,
			window.impl->window,
			context.impl->context
			))
			throw wm::Exception("Can't set Current context");
	}
}

