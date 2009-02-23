#ifndef WINDOW_IMPL_HPP
#define WINDOW_IMPL_HPP

#include <list>

#include <windows.h>

#include <wm/window.hpp>
#include <wm/pixelformat.hpp>

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
		HWND hwnd;

		std::list<EventHandler*> handlers;
	};
}

#endif
