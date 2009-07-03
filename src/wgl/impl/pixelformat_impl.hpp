#ifndef PIXELFORMAT_IMPL_HPP
#define PIXELFORMAT_IMPL_HPP

#include <wm/pixelformat.hpp>

namespace wm
{
	struct PixelFormat::impl_t
	{
		int index;

		int red, green, blue, alpha;
		int depth, stencil;
	};
}

#endif
