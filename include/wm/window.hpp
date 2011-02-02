#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <wm/pixelformat.hpp>
#include <wm/exception.hpp>

#include <wm/export.hpp>

namespace wm
{
	class Display;
	class Surface;
	class Context;
	class PixelFormat;
    class EventQueue;

	/// A Window
	class WM_EXPORT Window
	{
		public:
			/// Create a new Window
			/**
				@param display a display to create the window on
				@param width the width of the window
				@param height the height of the window
				@param format the pixel format of the window
                @param event_queue an event queue where events from this window go to
			*/
			Window(
				Display& display,
				unsigned int width,
				unsigned int height,
				const PixelFormat& format,
                EventQueue& event_queue);

			/// Destroy a window
			~Window();

			/// Show window
			void show();

			/// Hide window
			void hide();

			/// Get the size of a window
			/**
				@param width the width of the window gets written here
				@param height the width of the window gets written here
			*/
			void getSize(unsigned int &width, unsigned int &height);

			/// Resize window
			/**
				@param width the new width of the window
				@param height the new height of the window
			*/
			void resize(unsigned int width, unsigned int height);

			/// Set the title of a window (UTF-8)
			/**
				@param title the new title of the window, encoded in UTF-8
			*/
			void setTitle(const char* title); // utf-8

			/// Set this window to full screen
			/**
				Request the windowing system to change this window
				to full screen or windowed mode.

				The window size will be changed and a ResizeEvent will
				occur.

				@param full true if full screen, false for windowed
			*/
			void fullscreen(bool full);

			/// Move the mouse pointer inside this window
			/**
				The coordinates are relative to the window client area. If the
				given coordinates are outside the client area, no moving is
				performed.

				@param x horizontal position of the mouse cursor destination
				@param y vertical position of the mouse cursor destination
			*/
			void warpMouse(unsigned int x, unsigned int y);

			/// Show/Hide the mouse cursor
			/**
				@param show true to show mouse cursor, false to hide
			*/
			void showCursor(bool show);

			/// Set size limits for a window or make it unresizable
			/**
				If the width or height of the minimum or maximum size limit
				is equal to 0, that limit is disabled.

				If the minimum and maximum size limits are equal (and non-zero)
				the window is made un-resizable.

				If the minimum size is larger than the maximum size, the results
				are undefined.

				@param minW minimum width of the window
				@param minH minimum height of the window
				@param maxW maximum width of the window
				@param maxH maximum height of the window
			*/
			void setMinMaxSize(unsigned int minW, unsigned int minH, unsigned int maxW, unsigned int maxH);

			/// Request window repaint
			/**
				Ask the windowing system to be repainted. The windowing
				system will respond and an ExposeEvent will be generated
				when appropriate.

				If the width or the height of the dirty rectangle
				area is zero, the entire window be repainted.

				@param x the left side of the dirty rectangle
				@param y the top side of the dirty rectangle
				@param width the width of the dirty rectangle
				@param height the height of the dirty rectangle
				@see wm::ExposeEvent
			*/
			void repaint(unsigned int x = 0, unsigned int y = 0, unsigned int width = 0, unsigned int height = 0);

			/// Get the display of this window
			/**
				@return the Display on which this window was created
			*/
			Display& display() { return display_; }

			/// Get the surface attached to this window
			/**
				@return the surface attached to this window
				@throws wm::Exception if no surface is attached to this window
			*/
			Surface& surface() const
			{
				if(!surface_) throw wm::Exception("No Surface attached to Window");
				return *surface_;
			}

			/// Get the pixel format of this Window
			/**
				@return the pixel format of this window
			*/
			const PixelFormat& pixelformat() const { return pixelformat_; }

		private:
			Window(const Window&);
			Window& operator=(const Window&);

			struct impl_t;
			impl_t* impl;
			Display& display_;
			const PixelFormat& pixelformat_;
			Surface *surface_;

			friend class EventReader;
			friend class Display;
			friend class Context;
			friend class Configuration;
			friend class Surface;
			friend class PixelFormat;
			friend class CurrentContext;
	};
}

#undef WM_EXPORT

#endif

