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
	namespace glx
	{
		struct PixelFormatBuilder
		{
			virtual ~PixelFormatBuilder() {};
			virtual int numFormats() const = 0;
			virtual bool filterFormat(int index) const = 0;
			virtual wm::PixelFormat::Descriptor makeDescriptor(int index) const = 0;
			virtual void getVisual(int index, Visual *& visual, int &depth) const = 0;
			
			virtual XVisualInfo* getVisualInfo(int index) const = 0;
#ifdef GLX_VERSION_1_3
			virtual GLXFBConfig getFBConfig(int index) const = 0;
#endif
		};
	};

	class Display;

	struct Configuration::impl_t
	{
		glx::Extensions extensions;
		
		boost::scoped_ptr<const glx::PixelFormatBuilder> builder;
		
		std::list<PixelFormat::impl_t> formatdata;
		std::vector<PixelFormat> formats;
	};
}

#endif

