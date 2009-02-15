#include <iostream>
#include <cstdlib>

#include <windows.h>

#include <wm/exception.hpp>
#include <wm/display.hpp>
#include <wm/window.hpp>

#include "impl/error.hpp"
#include "impl/display_impl.hpp"
#include "impl/window_impl.hpp"

namespace wm
{
	Window::Window(
		Display& display,
		int,
		unsigned int width,
		unsigned int height,
		const PixelFormat& format)
		: impl(new impl_t(format))
		, display_(display)
	{
		int style = WS_OVERLAPPEDWINDOW;
		int exstyle = WS_EX_OVERLAPPEDWINDOW;

		RECT rect;
		if(!SetRect(&rect, 0, 0, width, height)) // how likely is this? ;)
			// NOTE: SetRect does not affect win32 GetLastError	
			throw Exception("Can't initialize window bounds rectangle");

		if(!AdjustWindowRectEx(&rect, style, false, exstyle))
			throw Exception("Can't adjust window bounds rectangle" + win32::getErrorMsg());

		impl->hwnd = CreateWindowEx(
			exstyle,
			display.impl->classname.c_str(),
			"Window",
			style,
			CW_USEDEFAULT, CW_USEDEFAULT,
			rect.right - rect.left, rect.bottom - rect.top,
			0,
			0,
			display.impl->hInstance,
			0);

		if(!impl->hwnd)
			throw Exception("Can't create Window: " + win32::getErrorMsg());
	}
	
	Window::~Window()
	{
		try
		{
			if(!DestroyWindow(impl->hwnd))
			{
				std::cerr << "Can't destroy window:	" << win32::getErrorMsg() << std::endl;
			}
		} catch(...)
		{
		}
	}
		
	void Window::show()
	{
		ShowWindow(impl->hwnd, SW_SHOWNORMAL);
	}

	void Window::hide()
	{
		ShowWindow(impl->hwnd, SW_HIDE);
	}
	
	void Window::swap()
	{
		HDC hdc = GetDC(impl->hwnd);
		if(!hdc)
			throw Exception("Can't get handle to win32 device context" + win32::getErrorMsg());

		if(!SwapBuffers(hdc))
		{
			DWORD err = GetLastError();
			ReleaseDC(impl->hwnd, hdc);
			throw Exception("Can't swap buffers: " + win32::getErrorMsg(err));
		}

		ReleaseDC(impl->hwnd, hdc);
	}
}
