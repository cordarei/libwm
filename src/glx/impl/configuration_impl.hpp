#ifndef CONFIGURATION_IMPL_HPP
#define CONFIGURATION_IMPL_HPP

#include <list>
#include <vector>

#include <boost/scoped_ptr.hpp>

#include <wm/configuration.hpp>

#include <glx/impl/pixelformat_impl.hpp>
#include <glx/impl/extensions.hpp>

#include <GL/glx.h>

namespace wm
{
	class Display;
	
	namespace glx
	{
		struct PixelFormatBuilder;
	}	

	struct Configuration::impl_t
	{
		~impl_t();
	
		glx::Extensions extensions;

		boost::scoped_ptr<const glx::PixelFormatBuilder> builder;
	
		std::list<PixelFormat::impl_t> formatdata;
		std::vector<PixelFormat> formats;
	};
}

#endif

