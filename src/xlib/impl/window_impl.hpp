#ifndef WINDOW_IMPL_HPP
#define WINDOW_IMPL_HPP

#include <X11/Xlib.h>
#include <GL/glx.h>

#include <wm/window.hpp>

#include <common/dispatcher.hpp>
#include <common/eventqueue.hpp>

#include <xlib/impl/eventreader.hpp>

namespace wm
{
	struct Window::impl_t
	{
		impl_t() : screen(0) {}

		int screen;
	
		EventReader eventreader;
		
		// PixelFormat information
		Visual *visual;
		int depth;
		
		// Color map
		Colormap colormap;		

		// Window handle
		::Window window;
		
		// X input context
		XIC xic;
		
		// Event dispatching
		common::Dispatcher dispatcher;
		common::EventQueue eventq;		
	};
}

#endif

