#ifndef CONTEXT_IMPL_HPP
#define CONTEXT_IMPL_HPP

#include <GL/glx.h>

#include <wm/context.hpp>

namespace wm
{
	class Configuration;

	namespace glx
	{
		struct Extensions;
	}

	struct Context::impl_t
	{
		explicit impl_t(Configuration& config)
			: config(config)
		{
		}
	
		Configuration& config;	
		GLXContext context;
	};
}

#endif

