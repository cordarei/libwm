#ifndef XLIB_IMPL_EVENTREADER_HPP
#define XLIB_IMPL_EVENTREADER_HPP

#include <X11/Xlib.h>

namespace wm
{
	class EventReader
	{
		public:
			EventReader()
				: width(0)
				, height(0)
				, keyrepeat_serial(0)
			{
			}
		
			static const long event_mask;
			
			static void handleXEvent(wm::Window& window, const XEvent &event);
					
		private:
		
            void handleClientMessage(wm::Window& window, const XEvent &event);
			void handleKeyEvent(wm::Window& window, const XEvent &event);
			void handleConfigureNotify(wm::Window& window, const XEvent &event);
			
			unsigned int width, height;
			unsigned long keyrepeat_serial;
	};	
}

#endif

