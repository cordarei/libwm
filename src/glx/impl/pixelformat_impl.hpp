#ifndef PIXELFORMAT_IMPL_HPP
#define PIXELFORMAT_IMPL_HPP

#include <wm/pixelformat.hpp>

#include <GL/glx.h>

namespace wm
{
	struct PixelFormat::impl_t
	{
		explicit impl_t(GLXFBConfig fbconfig = 0)
			: fbconfig(fbconfig)
		{
		}

		GLXFBConfig fbconfig;
	};
}

#endif

