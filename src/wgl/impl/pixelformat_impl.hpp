#ifndef PIXELFORMAT_IMPL_HPP
#define PIXELFORMAT_IMPL_HPP

#include <wm/pixelformat.hpp>

namespace wm
{
	struct PixelFormat::impl_t
	{
		explicit impl_t(int index = 0) : index(index) { }

		int index;
	};
}

#endif
