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

