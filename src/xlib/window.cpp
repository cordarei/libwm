#include <X11/Xlib.h>

#include <wm/pixelformat.hpp>
#include <wm/window.hpp>
#include "impl/window_impl.hpp"

namespace wm
{
	Window::Window(
		Display& display,
		int screen,
		unsigned int width,
		unsigned int height,
		const PixelFormat& format)
		: impl(new impl_t)
	{
		(void)display;
		(void)screen;
		(void)width;
		(void)height;
		(void)format;
		
		// TODO: implement me
	}
	
	Window::~Window()
	{
	}
}

