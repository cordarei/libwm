#include <iostream>

#include <windows.h>

#include <wm/exception.hpp>
#include <wm/display.hpp>
#include <wm/window.hpp>

#include <common/eventqueue.hpp>
#include "impl/error.hpp"
#include "impl/display_impl.hpp"
#include "impl/window_impl.hpp"
#include "impl/eventfactory.hpp"

namespace
{
	char getHexChar(unsigned int idx)
	{
		if(idx >= 16) return '!';
		return (idx < 10)
			? '0' + idx
			: 'a' + (idx - 10);
	}

	// Simple hex string representation of Display pointer
	std::string genClassNameStr(const wm::Display *display)
	{
		size_t size = sizeof(const wm::Display*);
		const unsigned char *begin =
			reinterpret_cast<const unsigned char *>(&display);

		std::string str;
		str.reserve(2 * size + 1);
		for(const unsigned char *ptr = begin;
			ptr != begin + size;
			++ptr)
		{
			str.push_back(getHexChar(*ptr & 0x0f)); // put least significant digit first for added obscurity
			str.push_back(getHexChar(*ptr & 0xf0 >> 4));
		}

		return str;
	}
}

namespace wm
{
	class EventReader
	{
		public:
			static LRESULT CALLBACK wndproc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
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

					const wm::Event *event = wm::win32::fromWin32Event(window, message, wparam, lparam);
					if(event)
					{
						window.impl->eventq.push(event);
						return TRUE;
					}
				}

				return DefWindowProc(hwnd, message, wparam, lparam);
			}
	};

	Display::Display(const char *name)
		: impl(new impl_t)
	{
		// Generate per-instance unique classname string
		impl->classname = genClassNameStr(this);

		// Get win32 module handle
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
		klass.lpfnWndProc = &EventReader::wndproc;
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

	void Display::dispatch(bool block)
	{
		if(block)
		{
			MSG msg;

			if(GetMessage(&msg, 0, 0, 0) == -1) // Yep, the return type is BOOL and it returns -1 on error
				throw Exception("Can't get win32 message: " + win32::getErrorMsg());

			TranslateMessage(&msg);

			if(DispatchMessage(&msg) < 0)
			{
				throw Exception("Can't dispatch message to window procedures: " + win32::getErrorMsg());
			}
		}
		
		// TODO: implement a non-blocking version
	}
}
