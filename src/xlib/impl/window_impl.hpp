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
		impl_t()
			: visual(0)
			, depth(0)
			, colormap(0)
			, window(0)
			, xic(0)
			, transparentCursor(0)
		{
		}
	
		// PixelFormat information
		Visual *visual;
		int depth;
		
		// Color map
		Colormap colormap;		

		// Window handle
		::Window window;
		
		// X input context
		XIC xic;
		
		// Transparent cursor
		Cursor transparentCursor;
		
		// Event dispatching
		common::Dispatcher dispatcher;
		common::EventQueue eventq;		

		EventReader eventreader;
	};
}

#endif

