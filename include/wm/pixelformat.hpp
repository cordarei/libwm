#ifndef PIXELFORMAT_HPP
#define PIXELFORMAT_HPP

#include <wm/export.hpp>

namespace wm
{
	class Configuration;

	class WM_EXPORT PixelFormat
	{
		public:
			struct WM_EXPORT Descriptor
			{
				Descriptor(int red, int green, int blue, int alpha, int depth, int stencil)
					: red(red)
					, green(green)
					, blue(blue)
					, alpha(alpha)
					, depth(depth)
					, stencil(stencil)
				{
				}
					
				
				int red, green, blue, alpha;
				int depth, stencil;
			};

			const Descriptor &descriptor() const { return descriptor_; }
			
		private:
			struct impl_t;
			impl_t *impl;
			Configuration *configuration;
			Descriptor descriptor_;
			
			PixelFormat(const Descriptor& descriptor, Configuration& configuration, impl_t &data)
				: impl(&data)
				, configuration(&configuration)
				, descriptor_(descriptor)
			{
			}
			
			friend class Configuration;
			friend class Surface;
			friend class Context;
	};
}

#undef WM_EXPORT

#endif

