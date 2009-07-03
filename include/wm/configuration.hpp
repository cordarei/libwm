#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <wm/export.hpp>

namespace wm
{
	class Display;
	class Window;
	class PixelFormat;

	class WM_EXPORT Configuration
	{
		public:
			explicit Configuration(Window& window);
			~Configuration();
			
			int numFormats() const;
			PixelFormat getFormat(int index) const;
	
			Display& display() const { return *display_; }

		private:
			Configuration(Configuration const &);
			Configuration& operator=(Configuration const &);
			
			struct impl_t;
			impl_t *impl;

			Display* display_;
			
			friend class Context;
	};
}

#undef WM_EXPORT

#endif

