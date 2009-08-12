#ifndef CONFIGURATION_IMPL_HPP
#define CONFIGURATION_IMPL_HPP

#include <list>
#include <vector>

#include <wm/configuration.hpp>

#include <glx/impl/pixelformat_impl.hpp>
#include <glx/impl/extensions.hpp>

#include <GL/glx.h>

namespace wm
{
	class Display;

	struct Configuration::impl_t
	{
		glx::Extensions extensions;
	
		std::list<PixelFormat::impl_t> formatdata;
		std::vector<PixelFormat> formats;
	};
}

#endif

