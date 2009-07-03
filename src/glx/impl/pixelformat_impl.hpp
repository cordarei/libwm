#ifndef PIXELFORMAT_IMPL_HPP
#define PIXELFORMAT_IMPL_HPP

#include <wm/pixelformat.hpp>

#include <GL/glx.h>

namespace wm
{
	struct PixelFormat::impl_t
	{
		impl_t()
			: fbconfig(0)
		{
		}

		GLXFBConfig fbconfig;
		int red, green, blue, alpha;
		int depth, stencil;

	};
}

#endif

