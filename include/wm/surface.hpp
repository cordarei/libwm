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
			
			/// Query surface size
			/**
				@param width a reference where to store the width of the surface
				@param height a reference where to store the height of the surface
			*/
			void getSize(unsigned int &width, unsigned int &height);
			
			/// Get the window of this surface
			Window& window() const { return *window_; }
		
		private:
			Surface(const Surface&);
			Surface& operator=(const Surface&);
		
			struct impl_t;
			impl_t *impl;
			Window *window_;	
			
            friend class CurrentContext;
	};
}

#undef WM_EXPORT

#endif

