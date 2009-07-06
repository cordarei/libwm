#ifndef CONFIGURATION_IMPL_HPP
#define CONFIGURATION_IMPL_HPP

#include <list>
#include <vector>

#include <wm/configuration.hpp>
#include <wm/pixelformat.hpp>

#include <wgl/impl/pixelformat_impl.hpp>
#include <wgl/impl/extensions.hpp>

namespace wm
{
	struct Configuration::impl_t
	{
		std::list<PixelFormat::impl_t> formatdata;
		std::vector<PixelFormat> formats;

		wgl::Extensions extensions;
	};
}

#endif
