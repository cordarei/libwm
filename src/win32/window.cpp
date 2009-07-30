#include <iostream>
#include <cstdlib>

#include <boost/scoped_array.hpp>

#include <windows.h>

#include <wm/exception.hpp>
#include <wm/display.hpp>
#include <wm/window.hpp>
#include <wm/pixelformat.hpp>

#include <win32/impl/error.hpp>
#include <win32/impl/display_impl.hpp>
#include <win32/impl/window_impl.hpp>

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
}

namespace wm
{
	Window::Window(
		Display& display,
		unsigned int width,
		unsigned int height,
		const PixelFormat &format)
		: impl(new impl_t)
		, display_(display)
		, pixelformat_(format)
	{
		std::auto_ptr<impl_t> impl_guard(impl); // deletes impl object in case of exception

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
					throw Exception("Can't set win32 Window user data: " + win32::getErrorMsg(err));
			}

			format.set(*this);

			// TrackMouseEvent to recieve WM_MOUSELEAVE events
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_LEAVE;
			tme.hwndTrack = impl->hwnd;

			impl->cursorVisible = true;

			if(!TrackMouseEvent(&tme))
				throw wm::Exception("Can't track mouse events: " + win32::getErrorMsg());
		} catch(...)
		{
			if(!DestroyWindow(impl->hwnd))
			{
				std::cerr << "Can't destroy window: " << win32::getErrorMsg() << std::endl;
			}

			throw;
		}

		impl_guard.release();
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

		if(!SetWindowTextW(impl->hwnd, array.get()))
			throw wm::Exception("Can't set window title: " + win32::getErrorMsg());
	}

	void Window::fullscreen(bool full)
	{
		if(full == impl->fullscreen) return;

		unsigned int minW = impl->minW, minH = impl->minH, maxW = impl->maxW, maxH = impl->maxH;

		RECT rect;

		if(full)
		{
			unsigned int width = GetSystemMetrics(SM_CXSCREEN);
			unsigned int height = GetSystemMetrics(SM_CYSCREEN);

			if(!width || !height)
				throw wm::Exception("Can't set full screen, GetSystemMetrics failed: " +
					win32::getErrorMsg());

			if(((minW != 0 && minH != 0) && (width < minW || height < minH)) ||
				((maxW != 0 && maxH != 0) && (width > maxW || height > maxH)))
				return;

			if(!SetRect(&rect, 0, 0, width, height))
				throw wm::Exception("Can't set window rectangle for resizing");
		} else
		{
			if(!SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0))
				throw wm::Exception("Can't set windowed, SystemParametersInfo failed: "
					+ win32::getErrorMsg());

			unsigned int width = rect.right - rect.left;
			unsigned int height = rect.bottom - rect.top;

			if(minW != 0 && minH != 0)
			{
				if(width < minW) rect.right = rect.left + minW;
				if(height < minH) rect.bottom = rect.top + minH;
			}

			if(maxW != 0 && maxH != 0)
			{
				if(width > maxW) rect.right = rect.left + maxW;
				if(height > maxH) rect.bottom = rect.top + maxH;
			}
		}

		LONG style = full ? WS_POPUP : WS_OVERLAPPEDWINDOW;
		LONG exstyle = full ? WS_EX_APPWINDOW : WS_EX_OVERLAPPEDWINDOW;

		hide();

		if(!SetWindowLong(impl->hwnd, GWL_EXSTYLE, exstyle)
			|| !SetWindowLong(impl->hwnd, GWL_STYLE, style))
			throw wm::Exception("Can't set full screen, SetWindowLong failed: " + win32::getErrorMsg());

		if(!SetWindowPos(
			impl->hwnd,
			HWND_TOPMOST,
			rect.left, rect.top,
			rect.right - rect.left, rect.bottom - rect.top,
			SWP_FRAMECHANGED | SWP_SHOWWINDOW))
			throw wm::Exception("Can't set full screen, SetWindowPos failed: " + win32::getErrorMsg());

		impl->style = style;
		impl->exstyle = exstyle;
		impl->fullscreen = full;
	}

	void Window::warpMouse(unsigned int x, unsigned int y)
	{
		RECT rect;
		if(!GetClientRect(impl->hwnd, &rect))
			throw wm::Exception("Can't warp mouse, GetClientRect failed: " + win32::getErrorMsg());

		if(LONG(x) >= rect.right || LONG(y) >= rect.bottom) return;

		POINT point = { 0, 0 };
		if(!ClientToScreen(impl->hwnd, &point))
			throw wm::Exception("Can't warp mouse, ClientToScreen failed: " + win32::getErrorMsg());

		SetCursorPos(point.x + x, point.y + y);
	}

	void Window::showCursor(bool show)
	{
		// TODO: synchronize this!
		// the critical section is between EventReader mouse stuff
		// and Window::impl_t::cursorVisible in Window::showCursor()

		if(impl->cursorVisible != show)
		{
			if(impl->eventreader.mouseinside)
			{
				ShowCursor(show);
			}

			impl->cursorVisible = show;
		}
	}

	void Window::setMinMaxSize(unsigned int minW, unsigned int minH, unsigned int maxW, unsigned int maxH)
	{
		// TODO: synchronize this!
		// The EventReader reads these on WM_GETMINMAXINFO
		// make sure the read is atomic
		impl->minW = minW; impl->minH = minH;
		impl->maxW = maxW; impl->maxH = maxH;
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
			
			do
			{
				impl->dispatcher.dispatch(*event);
			} while(impl->eventq.poll(event));
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
