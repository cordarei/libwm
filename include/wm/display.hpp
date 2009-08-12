#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <wm/export.hpp>

namespace wm
{
	class Window;
	class Context;
	class Surface;

	/// A display
	class WM_EXPORT Display
	{
		public:
			/// Open a display
			/**
				In the Xlib implementation, the Display constructor creates a new
				X Display connection, opens a new X Input Method and initializes
				Extended Window Manager Hints.
				
				The Win32 implementation of Display initializes a window class using RegisterClassExW.

				@param name an optional name parameter, used on some windowing systems
				@param screen optional screen parameter, used on some windowing systems, -1 for default
			*/	
			explicit Display(const char *name = 0, int screen = -1);
			
			/// Close display
			~Display();
			
			/// Wait for events from the windowing system and add events to window event queues
			/**
				This function listens for events from the windowing
				system, processes them and adds events to the event queues
				of the appropriate Windows. This function may add zero or more
				events to event queues, because windowing system
				events and Libwm events don't have a one-to-one correspondance.

				If no windowing system events are available, this function waits
				until an event arrives.

				@see Display::poll
				@see Window::wait
				@see Window::dispatch
			*/
			void wait();
			
			/// Poll for events from the windowing system and add them to window event queues
			/**
				This function listens for events from the windowing
				system, processes them and adds events to the event queues
				of the appropriate Windows. This function may add zero or more
				events to event queues, because windowing system
				events and Libwm events don't have a one-to-one correspondance.

				If no windowing system events are available, this function returns
				immediately.

				Processing the events may take an indefinate amount of time and may
				be affected by user interaction.

				@see Display::wait
				@see Window::wait
				@see Window::dispatch
			*/
			void poll();
						
		private:
			Display(const Display&);
			Display& operator=(const Display&);

			struct impl_t;
			impl_t* impl;
			
			friend class EventReader;
			friend class Window;
			friend class Context;
			friend class Configuration;
			friend class Surface;
			friend class PixelFormat;
			
			friend void WM_EXPORT makeCurrent(Context &context, Surface &draw, Surface &read);
	};
}

#undef WM_EXPORT

#endif


