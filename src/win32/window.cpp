#include <iostream>
#include <cstdlib>

#include <boost/scoped_array.hpp>

#include <windows.h>

#include <wm/exception.hpp>
#include <wm/display.hpp>
#include <wm/window.hpp>

#include "impl/error.hpp"
#include "impl/display_impl.hpp"
#include "impl/window_impl.hpp"

namespace
{
	HWND createWindow(HINSTANCE hInstance, const WCHAR *wndclass, int width, int height, int style, int exstyle)
	{
		RECT rect;
		if(!SetRect(&rect, 0, 0, width, height)) // how likely is this? ;)
			// NOTE: SetRect does not affect win32 GetLastError	
			throw wm::Exception("Can't initialize window bounds rectangle");

		if(!AdjustWindowRectEx(&rect, style, false, exstyle))
			throw wm::Exception("Can't adjust window bounds rectangle" + wm::win32::getErrorMsg());

		WCHAR nullterm = 0;
		HWND hwnd = CreateWindowExW(
			exstyle,
			wndclass,
			&nullterm,	// window title
			style,
			CW_USEDEFAULT, CW_USEDEFAULT,
			rect.right - rect.left, rect.bottom - rect.top,
			0,
			0,
			hInstance,
			0);

		if(!hwnd)
			throw wm::Exception("Can't create Window: " + wm::win32::getErrorMsg());

		return hwnd;
	}

	void setPixelFormat(HWND hwnd, const wm::PixelFormat& format)
	{
		HDC hdc = GetDC(hwnd);
		if(!hdc)
			throw wm::Exception("Can't get win32 device context" + wm::win32::getErrorMsg());

		PIXELFORMATDESCRIPTOR pfd;
		std::memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.dwFlags =
			PFD_DRAW_TO_WINDOW |
			PFD_SUPPORT_OPENGL |
			PFD_DOUBLEBUFFER;

		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cRedBits = format.red;
		pfd.cGreenBits = format.green;
		pfd.cBlueBits = format.blue;
		pfd.cAlphaBits = format.alpha;
		pfd.cDepthBits = format.depth;
		pfd.cStencilBits = format.stencil;

		int fmt = ChoosePixelFormat(hdc, &pfd);
		if(!fmt)
		{
			DWORD err = GetLastError();
			ReleaseDC(hwnd, hdc);
			throw wm::Exception("Can't choose pixel format" + wm::win32::getErrorMsg(err));
		}
		
		if(!SetPixelFormat(hdc, fmt, &pfd))
		{
			DWORD err = GetLastError();
			ReleaseDC(hwnd, hdc);
			throw wm::Exception("Can't set pixel format" + wm::win32::getErrorMsg(err));
		}

		ReleaseDC(hwnd, hdc);
	}
}

namespace wm
{
	Window::Window(
		Display& display,
		int,
		unsigned int width,
		unsigned int height,
		const PixelFormat& format)
		: impl(new impl_t)
		, display_(display)
	{
		impl->style = WS_OVERLAPPEDWINDOW;
		impl->exstyle = WS_EX_OVERLAPPEDWINDOW;

		impl->hwnd = createWindow(
			display.impl->hInstance,
			&(display.impl->classname)[0],
			width,
			height,
			impl->style,
			impl->exstyle);

		try
		{
			SetLastError(0);
			if(!SetWindowLongPtr(impl->hwnd, 0, reinterpret_cast<LONG_PTR>(this)))
			{
				DWORD err = GetLastError();
				if(err != 0)
					throw Exception("Can't set win32 Window user data" + win32::getErrorMsg(err));
			}

			setPixelFormat(impl->hwnd, format);
		} catch(...)
		{
			if(!DestroyWindow(impl->hwnd))
			{
				std::cerr << "Can't destroy window" << win32::getErrorMsg() << std::endl;
			}

			throw;
		}
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

		delete impl;
	}
		
	void Window::show()
	{
		ShowWindow(impl->hwnd, SW_SHOWNORMAL);
	}

	void Window::hide()
	{
		ShowWindow(impl->hwnd, SW_HIDE);
	}

	void Window::getSize(unsigned int &width, unsigned int &height)
	{
		RECT rect;
		if(!GetClientRect(impl->hwnd, &rect))
			throw wm::Exception("Can't get win32 window client area size: " + win32::getErrorMsg());

		unsigned int w = rect.right - rect.left;
		unsigned int h = rect.bottom - rect.top;

		width = w;
		height = h;
	}

	void Window::resize(unsigned int width, unsigned int height)
	{
		RECT rect;
		if(!SetRect(&rect, 0, 0, width, height))
			throw wm::Exception("Can't set window rectangle for resizing");

		int style = impl->style,
			exstyle = impl->exstyle;

		if(!AdjustWindowRectEx(&rect, style, false, exstyle))
			throw wm::Exception("Can't adjust window bounds rectangle: " + wm::win32::getErrorMsg());

		if(!SetWindowPos(
			impl->hwnd,
			0, // HWND hWndInsertAfter
			0, 0, // x, y (ignored because of SWP_NOMOVE)
			rect.right - rect.left,
			rect.bottom - rect.top,
			SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE))
			throw wm::Exception("Can't resize win32 window: " + win32::getErrorMsg());
	}

	void Window::setTitle(const char *title)
	{
		if(!title) return;
		
		int len = 0;
		boost::scoped_array<WCHAR> array(0);
		for(int i = 0; i < 2; ++i)
		{
			len = MultiByteToWideChar(
				CP_UTF8,
				MB_ERR_INVALID_CHARS, // fail if invalid characters
				title,
				-1, // length of input = -1  <=>  string is NULL-terminated
				array.get(), // output buffer, NULL on the first pass <=> MultiByteToWideChar calculates size of output
				len // output buffer size, in WCHAR's
				);

			if(!len)
				throw Exception("MultiByteToWideChar failed: " + win32::getErrorMsg()); 

			if(!array) array.reset(new WCHAR[len]);
		} 

		SetWindowTextW(impl->hwnd, array.get());
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

#include <wm/event.hpp>

namespace wm
{
	void Window::dispatch(bool block)
	{
		if(block)
		{
			boost::scoped_ptr<const Event> event;

			while(!impl->eventq.poll(event))
			{
				display().dispatch(true);
			}
			
			impl->dispatcher.dispatch(*event);
		} else
		{
			boost::scoped_ptr<const Event> event;
			while(impl->eventq.poll(event))
			{
				impl->dispatcher.dispatch(*event);
			}
		}		
	}

	common::Dispatcher& Window::dispatcher()
	{
		return impl->dispatcher;
	}
}
