#ifndef CONFIGURATION_IMPL_HPP
#define CONFIGURATION_IMPL_HPP

#include <vector>

#include <wm/configuration.hpp>
#include <wm/pixelformat.hpp>

#include <wgl/impl/pixelformat_impl.hpp>

namespace wm
{
	struct Configuration::impl_t
	{
		std::vector<PixelFormat::impl_t> formatdata;
	};
}

#endif
