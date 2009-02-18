#ifndef WINDOW_IMPL_HPP
#define WINDOW_IMPL_HPP

#include <list>

#include <X11/Xlib.h>
#include <GL/glx.h>

#include <wm/window.hpp>

namespace wm
{
	class EventHandler;
	
	class Window::ConnectionInfo : boost::noncopyable
	{
			public:
				ConnectionInfo() : connected(false) {}
			private:
				bool connected;
				std::list<EventHandler*>::iterator iterator;
				
				friend class Window;
	};

	struct Window::impl_t
	{
		::Window window;
		XVisualInfo *visualinfo;
		long event_mask;
		
		std::list<EventHandler*> handlers;
	};
}

#endif

