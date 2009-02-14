#ifndef CONTEXT_IMPL_HPP
#define CONTEXT_IMPL_HPP

#include <GL/glx.h>

#include <wm/context.hpp>

namespace wm
{
	struct Context::impl_t
	{
		GLXContext context;
	};
}

#endif

