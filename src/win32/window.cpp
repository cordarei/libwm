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

#include <algorithm>

#include <wm/event.hpp>

#include <boost/bind.hpp>
#include <boost/ref.hpp>

namespace
{
	typedef std::list<wm::EventHandler*> HandlerList;
	
	void dispatch(
		const HandlerList &handlers,
		const wm::Event &event
		)
	{
		using boost::bind;
		using boost::ref;
		
		for(HandlerList::const_iterator i = handlers.begin();
			i != handlers.end();
			++i)
		{
			wm::EventHandler& handler = **i;
			event.accept(handler);
		}

	}
}

namespace wm
{
	void Window::dispatch(bool block)
	{
		if(block)
		{
			MSG msg;
			if(GetMessage(&msg, impl->hwnd, 0, 0) == -1) // Yep, the return type is BOOL and it returns -1 on error
			{
				throw Exception("Can't get win32 message: " + win32::getErrorMsg());
			}

			if(msg.message == WM_KEYDOWN)
			{
				using boost::bind;
				using boost::ref;

				wm::KeyEvent event(*this, true);
				::dispatch(impl->handlers, event);
			}
		}
	}

	void Window::connect(EventHandler &handler, ConnectionInfo &info)
	{
		{
			// TODO: Synchronization goes here
			if(info.connected) return;
			
			info.iterator = impl->handlers.insert(impl->handlers.begin(), &handler);
			info.connected = true;
		}
	}

	void Window::disconnect(ConnectionInfo &info)
	{
		{
			// TODO: Synchronization goes here
			if(!info.connected) return;		
			
			impl->handlers.erase(info.iterator);
			info.connected = false;
		}
	}
	
	bool Window::isConnected(ConnectionInfo &info)
	{
		{
			// TODO: Synchronization goes here
			return info.connected;
		}
	}
}
