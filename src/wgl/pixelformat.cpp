#include <wm/pixelformat.hpp>
#include <wgl/impl/pixelformat_impl.hpp>

namespace wm
{
	int PixelFormat::red() const { return impl->red; }
	int PixelFormat::green() const { return impl->green; }
	int PixelFormat::blue() const { return impl->blue; }
	int PixelFormat::alpha() const { return impl->alpha; }

	int PixelFormat::depth() const { return impl->depth; }
	int PixelFormat::stencil() const { return impl->stencil; }
}

