#ifndef CONFIGURATION_IMPL_HPP
#define CONFIGURATION_IMPL_HPP

#include <list>
#include <vector>

#include <wm/configuration.hpp>

#include <glx/impl/pixelformat_impl.hpp>

#include <GL/glx.h>

namespace wm
{
	class Display;

	struct Configuration::impl_t
	{
		impl_t()
			: versionMajor(0)
			, versionMinor(0)
		{
		}

		int versionMajor, versionMinor;
		std::list<PixelFormat::impl_t> formatdata;
		std::vector<PixelFormat> formats;
	};
}

#endif

