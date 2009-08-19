#ifndef CONTEXT_IMPL_HPP
#define CONTEXT_IMPL_HPP

#include <GL/glx.h>

#include <wm/context.hpp>

namespace wm
{
	namespace glx
	{
		struct Extensions;
	}

	struct Context::impl_t
	{
		const glx::Extensions *extensions;
	
		GLXContext context;
	};
}

#endif

