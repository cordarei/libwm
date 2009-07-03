#ifndef SURFACE_IMPL_HPP
#define SURFACE_IMPL_HPP

#include <wm/surface.hpp>

#include <wm/eventhandler.hpp>
#include <wm/connection.hpp>

#include <X11/Xlib.h>
#include <GL/glx.h>

namespace wm
{
	class Window;

	struct Surface::impl_t
	{
		explicit impl_t(Window& window)
			: window(&window)
			, handler(ResizeHandler())
			, connection(window, handler, false)
		{
		}
	
		Window *window;
	
		struct ResizeHandler : public EventHandler
		{
			virtual void handle(const ResizeEvent& event);
		} handler;
		
		Connection connection;

		Colormap colormap;
		::Window subwindow;
		GLXWindow glxwindow;
		
	private:
		impl_t(impl_t const&);
		impl_t& operator=(impl_t const&);
	};
}

#endif

