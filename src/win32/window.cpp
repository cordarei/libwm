#include <iostream>
#include <cstdlib>

#include <windows.h>

#include <wm/exception.hpp>
#include <wm/display.hpp>
#include <wm/window.hpp>

#include "impl/error.hpp"
#include "impl/display_impl.hpp"
#include "impl/window_impl.hpp"

namespace
{
	HWND createWindow(HINSTANCE hInstance, const char *wndclass, int width, int height)
	{
		int style = WS_OVERLAPPEDWINDOW;
		int exstyle = WS_EX_OVERLAPPEDWINDOW;

		RECT rect;
		if(!SetRect(&rect, 0, 0, width, height)) // how likely is this? ;)
			// NOTE: SetRect does not affect win32 GetLastError	
			throw wm::Exception("Can't initialize window bounds rectangle");

		if(!AdjustWindowRectEx(&rect, style, false, exstyle))
			throw wm::Exception("Can't adjust window bounds rectangle" + wm::win32::getErrorMsg());

		HWND hwnd = CreateWindowEx(
			exstyle,
			wndclass,
			"Window",
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
		impl->hwnd = createWindow(
			display.impl->hInstance,
			display.impl->classname.c_str(),
			width,
			height);

		try
		{
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

#include <wm/event.hpp>
#include "impl/dispatch_impl.hpp"

namespace wm
{
	void Window::dispatch(bool block)
	{
		if(block)
		{
			MSG msg;
			if(GetMessage(&msg, impl->hwnd, 0, 0) == -1) // Yep, the return type is BOOL and it returns -1 on error
				throw Exception("Can't get win32 message: " + win32::getErrorMsg());

			if(DispatchMessage(&msg) < 0)
			{
				throw Exception("Can't dispatch message to window procedures: " + win32::getErrorMsg());
			}

			if(msg.message == WM_PAINT)
			{
				// hack du jour:
				// Since we're not using GDI, stop win32 from sending more WM_PAINT messages
				// (usually implicitly done by GDI)
				RECT rect, *ptr = 0;
				if(GetUpdateRect(impl->hwnd, &rect, FALSE)) ptr = &rect;
				if(!ValidateRect(impl->hwnd, ptr)) // if ptr == NULL, validates entire window
					throw Exception("Can't validate win32 window rectangle" + win32::getErrorMsg());
			}

			win32::dispatchEvent(*this, dispatcher(), msg);			
		}
		
		// TODO: implement a non-blocking version
	}

	common::Dispatcher& Window::dispatcher()
	{
		return impl->dispatcher;
	}
}
