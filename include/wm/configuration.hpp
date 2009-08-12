#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <wm/export.hpp>

namespace wm
{
	class Display;
	class Window;
	class PixelFormat;

	/// Graphics configuration
	/**
	*/
	class WM_EXPORT Configuration
	{
		public:
			/// Get the graphics configuration of a display
			/**
				In the GLX/Xlib implementation the Configuration constructor queries
				the GLX version, dynamically loads GLX functions and extensions.
				If GLX 1.3 is supported, the configuration uses GLXFBConfigs, otherwise
				XVisualInfos are used for backward compatibility.

				The WGL/Win32 implementation creates a dummy window and a dummy context
				to query available WGL extensions and the pixel formats. WGL_ARB_pixel_format
				is used when available, DescribePixelFormat is used for compatibility if not.
			
				@param display the display to get the graphics configuration from
			*/
			explicit Configuration(Display& display);
			
			/// Destroy Configuration
			~Configuration();
			
			/// Number of available pixel formats
			/**
				@return the number of available pixel formats
			*/
			int numFormats() const;
			
			/// Get a pixel format
			/**
				@return a reference to a supported pixel format
			*/
			const PixelFormat& getFormat(int index) const;
	
			/// The Display associated with this Configuration
			Display& display() const { return *display_; }

		private:
			Configuration(Configuration const &);
			Configuration& operator=(Configuration const &);
			
			struct impl_t;
			impl_t *impl;

			Display* display_;
			
			friend class Surface;
			friend class Context;
			friend class PixelFormat;
	};
}

#undef WM_EXPORT

#endif

