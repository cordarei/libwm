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
				@param name an optional name parameter, used on some windowing systems
			*/	
			explicit Display(const char *name = 0);
			
			/// Close display
			~Display();
			
			/// Listen for events and add them to the event queue
			/**
				This function listens for events from the windowing
				system and adds them to the event queue of the
				appropriate Windows.
				
				This function can be used in blocking or non-blocking
				mode. In blocking mode, this function waits for an event
				to arrive while in non-blocking mode the function returns
				immediately if there are no events available.
				
				On some platforms, this function may block depending on
				user input. For example, this function does not return
				while the user is resizing a window.
			
				@param block true if this function should wait for events
			*/
			void dispatch(bool block);
			
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
			
			friend void WM_EXPORT makeCurrent(Context &context, Surface &draw, Surface &read);
	};
}

#undef WM_EXPORT

#endif


