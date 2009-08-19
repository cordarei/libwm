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
				@param versionMajor major OpenGL version of the context
				@param versionMinor minor OpenGL version of the context
				@param compatible enable OpenGL backward compatibility
				@param debug create a debugging context
				@param direct attempt to use direct rendering
				@param shared a pointer to a Context to share resources
					(buffers, textures, shaders, etc) with or 0 for 
					no shared context
				@see Context::direct
			*/
			explicit Context(
				const PixelFormat &format,
				int versionMajor = 1,
				int versionMinor = 0,
				bool compatible = true,
				bool debug = false,
				bool direct = true,
				Context *shared = 0);
			
			/// Destroy a rendering context
			~Context();
			
			/// Get the display associated with this rendering context
			Display& display() const { return *display_; }

			/// Check if the context supports direct rendering
			/**
				@return true if the direct rendering is supported
					or the underlying platform doesn't distinguish indirect rendering
			*/
			bool direct() const;
			
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

