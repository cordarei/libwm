#ifndef WINDOW_IMPL_HPP
#define WINDOW_IMPL_HPP

#include <X11/Xlib.h>
#include <GL/glx.h>

#include <wm/window.hpp>

#include <common/dispatcher.hpp>
#include <common/eventqueue.hpp>

namespace wm
{
	struct Window::impl_t
	{
		impl_t() : screen(0), width(0), height(0) {}

		int screen;
	
		// Used by EventReader when dispatching resize events
		unsigned int width, height;

		// Window handle
		::Window window;
		XVisualInfo *visualinfo;
		
		// X input context
		XIC xic;
		
		// Event dispatching
		common::Dispatcher dispatcher;
		common::EventQueue eventq;		
	};
}

#endif

