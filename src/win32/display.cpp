#include <iostream>

#include <windows.h>

#include <wm/exception.hpp>
#include <wm/display.hpp>

#include "impl/error.hpp"
#include "impl/display_impl.hpp"

namespace
{
	LRESULT CALLBACK wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		return DefWindowProc(hwnd, msg, wparam, lparam);
	};
}

namespace wm
{
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
		klass.cbWndExtra = 0;
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
