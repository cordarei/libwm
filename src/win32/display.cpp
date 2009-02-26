#include <iostream>

#include <windows.h>

#include <wm/exception.hpp>
#include <wm/display.hpp>
#include <wm/window.hpp>

#include "impl/error.hpp"
#include "impl/display_impl.hpp"
#include "impl/dispatch_impl.hpp"

namespace
{
	LRESULT CALLBACK wndproc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
	{
		SetLastError(0);
		wm::Window *ptr =
			reinterpret_cast<wm::Window*>(
				GetWindowLongPtr(hwnd, 0));
		if(!ptr)
		{
			DWORD err = GetLastError();
			if(err)
				throw wm::Exception("Can't get win32 window user data" +
					wm::win32::getErrorMsg(err));
		} else
		{
			wm::Window& window = *ptr;

			if(message == WM_PAINT)
			{
				// ValidateRect prevents Windows from resending WM_PAINT
				RECT rect, *ptr = 0;
				if(GetUpdateRect(hwnd, &rect, FALSE)) ptr = &rect;
				if(!ValidateRect(hwnd, ptr)) // if ptr == NULL, validates entire window
					throw wm::Exception("Can't validate win32 window rectangle" + wm::win32::getErrorMsg());
			}

			if(wm::win32::dispatchEvent(window, window.dispatcher(), message, wparam, lparam))
				return TRUE;
		}

		return DefWindowProc(hwnd, message, wparam, lparam);
	};
}

namespace wm
{
	class Window;

	Display::Display(const char *name)
		: impl(new impl_t("wmwinwdow"))
	{
		impl->hInstance = GetModuleHandle(0);
		if(!impl->hInstance)
			throw Exception("Could not get win32 module handle: " + win32::getErrorMsg());

		// Load cursor
		HCURSOR hcursor = LoadCursor(0, IDC_ARROW);
		if(!hcursor)
			throw Exception("Could not load win32 default cursor" + win32::getErrorMsg());

		WNDCLASSEX klass;
		klass.cbSize = sizeof(WNDCLASSEX);
		klass.style = 0;
		klass.lpfnWndProc = &wndproc;
		klass.cbClsExtra = 0;
		klass.cbWndExtra = sizeof(Window*);
		klass.hInstance = impl->hInstance;
		klass.hIcon = 0;
		klass.hCursor = hcursor;
		klass.hbrBackground = 0;
		klass.lpszMenuName = 0;
		klass.lpszClassName = impl->classname.c_str();
		klass.hIconSm = 0;

		if(!RegisterClassEx(&klass))
			throw Exception("Can't register win32 window class: " + win32::getErrorMsg());
	}

	Display::~Display()
	{
		try
		{
			if(!UnregisterClass(impl->classname.c_str(), impl->hInstance))
			{
				std::cerr << "Can't unregister win32 window class: " << win32::getErrorMsg();
			}
		} catch(...)
		{
		}
	}
}
