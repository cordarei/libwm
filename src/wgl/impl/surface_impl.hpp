#ifndef SURFACE_IMPL_HPP
#define SURFACE_IMPL_HPP

#include <wm/surface.hpp>

namespace wm
{
	class Window;

	struct Surface::impl_t
	{
		Window *window;
	};
}

#endif
