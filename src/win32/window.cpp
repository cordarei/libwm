#include <cstdlib>

#include <windows.h>

#include <wm/exception.hpp>
#include <wm/window.hpp>

#include "impl/error.hpp"
#include "impl/window_impl.hpp"

namespace
{
	LRESULT CALLBACK wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		return DefWindowProc(hwnd, msg, wparam, lparam);
	};
}

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
		HINSTANCE hInstance = GetModuleHandle(NULL);

		WNDCLASSEX klass;
		std::memset(&klass, 0, sizeof(WNDCLASSEX));

		klass.cbSize = sizeof(WNDCLASSEX);
		klass.lpfnWndProc = &wndproc;
		klass.cbClsExtra = 0;
		klass.cbWndExtra = 0;
		klass.hInstance = hInstance;
		klass.hIcon = 0;
		klass.hIconSm = 0;
		klass.hCursor = LoadCursor(0, IDC_ARROW);
		klass.hbrBackground = 0;
		klass.lpszMenuName = 0;
		klass.lpszClassName = "wmwindow";

		if(!RegisterClassEx(&klass))
			throw Exception("Can't register Window class: " + win32::getErrorMsg());

		int style = WS_OVERLAPPEDWINDOW;
		int exstyle = WS_EX_OVERLAPPEDWINDOW;

		RECT rect;
		SetRect(&rect, 0, 0, width, height);
		AdjustWindowRectEx(&rect, style, false, exstyle);

		impl->hwnd = CreateWindowEx(
			exstyle,
			"wmwindow",
			"Window",
			style,
			CW_USEDEFAULT, CW_USEDEFAULT,
			rect.right - rect.left, rect.bottom - rect.top,
			0,
			0,
			hInstance,
			0);

		if(!impl->hwnd)
			throw Exception("Can't create Window");
	}
	
	Window::~Window()
	{
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
		SwapBuffers(hdc);
		ReleaseDC(impl->hwnd, hdc);
	}
}
