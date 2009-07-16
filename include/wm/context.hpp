#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <wm/export.hpp>

namespace wm
{
	class Window;
	class Display;
	class Surface;

	/// A rendering context
	class WM_EXPORT Context
	{
		public:
			/// Create a new rendering context
			/**
				@param format the pixel format to use with the rendering context
				@param shared a pointer to a Context to share resources
					(buffers, textures, shaders, etc) with or 0 for 
					no shared context
			*/
			explicit Context(const PixelFormat &format, Context *shared = 0);
			
			/// Destroy a rendering context
			~Context();
			
			/// Get the display associated with this rendering context
			Display& display() const { return *display_; }
			
		private:
			Context(const Context&);
			Context& operator=(const Context&);

			struct impl_t;
			impl_t* impl;
			
			Display *display_;

			friend void WM_EXPORT makeCurrent(Context &context, Surface &draw, Surface &read);
	};
	
	/// Set current context of the calling thread
	/**
		@param context the rendering context to use
		@param draw the surface to draw to
		@param read the surface where to read from
	*/
	void WM_EXPORT makeCurrent(Context &context, Surface &draw, Surface &read);
}

#undef WM_EXPORT

#endif

