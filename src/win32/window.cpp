#include <wm/window.hpp>

namespace wm
{
	struct Window::impl_t
	{
	};

	Window::Window(
		Display& display,
		int screen,
		unsigned int width,
		unsigned int height,
		const PixelFormat& format)
		: impl(0)
		, display_(display)
	{
	}
	
	Window::~Window()
	{
	}
		
	void Window::show()
	{
	}

	void Window::hide()
	{
	}
	
	void Window::swap()
	{
	}
}
