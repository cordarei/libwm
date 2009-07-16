#ifndef WM_SURFACE_HPP
#define WM_SURFACE_HPP

#include <wm/export.hpp>

namespace wm
{
	class Window;
	class PixelFormat;
	class Context;

	/// A rendering surface
	class WM_EXPORT Surface
	{
		public:
			/// Create a new rendering surface
			/**
				Create a new rendering surface associated to a window.
				A window can have only attached surface. After the
				surface is deleted, a new surface cannot be attached
				to the same window.

				@param window the window to draw to
			*/
			explicit Surface(Window &window);
			
			/// Destroy a rendering surface
			~Surface();
		
			/// Swap buffers
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

