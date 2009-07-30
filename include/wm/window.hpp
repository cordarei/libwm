#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <wm/pixelformat.hpp>
#include <wm/connection.hpp>
#include <wm/exception.hpp>

#include <wm/export.hpp>

namespace wm
{
	class Display;
	class EventHandler;
	class Surface;
	class Context;
	class PixelFormat;
	
	namespace common
	{
		class Dispatcher;
	}
	
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
			*/
			Window(
				Display& display,
				unsigned int width,
				unsigned int height,
				const PixelFormat& format);
			
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
			
			/// Read events from the event queue and call event handlers
			/**
				Reads events from window-specific event queues and calls
				all event handlers connected to this window. This function
				can be used in blocking or non-blocking mode. In blocking
				mode this function waits for events if none are available
				in the event queue. The non-blocking version returns
				immediately if no events are available.
			
				In blocking mode, this function calls Display::dispatch(true)
				until an event arrives for this window.
			
				@param block true if this function should wait for events
				@see wm::Display::dispatch
				@see wm::EventHandler
				@see wm::Connection
			*/			
			void dispatch(bool block);

		private:
			Window(const Window&);
			Window& operator=(const Window&);

			struct impl_t;
			impl_t* impl;
			Display& display_;
			const PixelFormat& pixelformat_;
			Surface *surface_;

			common::Dispatcher &dispatcher();

			friend class EventReader;
			friend class Connection;
			friend class Display;
			friend class Context;
			friend class Configuration;
			friend class Surface;
			friend class PixelFormat;

			friend void WM_EXPORT makeCurrent(Context &context, Surface &draw, Surface &read);
	};
}

#undef WM_EXPORT

#endif

