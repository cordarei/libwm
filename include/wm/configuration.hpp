#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <wm/export.hpp>

namespace wm
{
	class Window;

	class WM_EXPORT Configuration
	{
		public:
			explicit Configuration(Window& window);
			~Configuration();
			
			int numFormats() const;
			PixelFormat getFormat(int index) const;
	
		private:
			Configuration(Configuration const &);
			Configuration& operator=(Configuration const &);
			
			struct impl_t;
			impl_t *impl;
			
			friend class Context;
	};
}

#undef WM_EXPORT

#endif

