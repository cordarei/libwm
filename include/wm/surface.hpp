#ifndef WM_SURFACE_HPP
#define WM_SURFACE_HPP

#include <wm/export.hpp>

namespace wm
{
	class Window;
	class PixelFormat;
	class Context;

	class WM_EXPORT Surface
	{
		public:
			Surface(Window &window, PixelFormat const &format);
			~Surface();
		
			void swap();
		
		private:
			Surface(const Surface&);
			Surface& operator=(const Surface&);
		
			struct impl_t;
			impl_t *impl;			

			friend void WM_EXPORT makeCurrent(Context &context, Surface &draw, Surface &read);
	};
}

#undef WM_EXPORT

#endif

