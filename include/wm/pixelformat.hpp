#ifndef PIXELFORMAT_HPP
#define PIXELFORMAT_HPP

#include <wm/export.hpp>

namespace wm
{
	class Configuration;

	class WM_EXPORT PixelFormat
	{
		public:
			int red() const;
			int green() const;
			int blue() const;
			int alpha() const;
			int depth() const;
			int stencil() const;
			
		private:
			struct impl_t;
			impl_t *impl;
			const Configuration *configuration;
			
			PixelFormat(const Configuration& configuration, impl_t *data)
				: impl(data)
				, configuration(&configuration)
			{
			}
			
			friend class Configuration;
			friend class Surface;
			friend class Context;
	};
}

#undef WM_EXPORT

#endif

