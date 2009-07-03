#ifndef CONFIGURATION_IMPL_HPP
#define CONFIGURATION_IMPL_HPP

#include <vector>

#include <wm/configuration.hpp>

#include <glx/impl/pixelformat_impl.hpp>

#include <GL/glx.h>

namespace wm
{
	class Display;

	struct Configuration::impl_t
	{
		explicit impl_t(Display& display)
			: display(&display)
			, versionMajor(0)
			, versionMinor(0)
		{
		}

		Display *display;	
		int versionMajor, versionMinor;
		std::vector<PixelFormat::impl_t> formatdata;
	};
}

#endif

