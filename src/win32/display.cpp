#include <iostream>
#include <vector>

#include <wm/exception.hpp>
#include <wm/display.hpp>
#include <wm/window.hpp>
#include <wm/event.hpp>
#include <wm/events.hpp>

#include <common/eventqueue.hpp>
#include <win32/impl/classname.hpp>
#include <win32/impl/error.hpp>
#include <win32/impl/display_impl.hpp>
#include <win32/impl/eventreader.hpp>

#include <windows.h>

namespace wm
{
	Display::Display(const char*, int)
		: impl(new impl_t)
	{
		std::auto_ptr<impl_t> impl_guard(impl); // deletes impl object in case of exception

		// Generate per-instance unique classname string
		WCHAR classname[sizeof(Display*) * 2 + 1];
		win32::genClassNameStr(this, classname);

		// Get win32 module handle
		impl->hInstance = GetModuleHandle(0);
		if(!impl->hInstance)
			throw Exception("Could not get win32 module handle: " + win32::getErrorMsg());

		// Load cursor
		HCURSOR hcursor = LoadCursor(0, IDC_ARROW);
		if(!hcursor)
			throw Exception("Could not load win32 default cursor" + win32::getErrorMsg());

		WNDCLASSEXW klass;
		klass.cbSize = sizeof(WNDCLASSEXW);
		klass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		klass.lpfnWndProc = &EventReader::WndProc;
		klass.cbClsExtra = 0;
		klass.cbWndExtra = sizeof(Window*);
		klass.hInstance = impl->hInstance;
		klass.hIcon = 0;
		klass.hCursor = hcursor;
		klass.hbrBackground = 0;
		klass.lpszMenuName = 0;
		klass.lpszClassName = classname;
		klass.hIconSm = 0;

		impl->classatom = RegisterClassExW(&klass);
		if(!impl->classatom)
			throw Exception("Can't register win32 window class: " + win32::getErrorMsg());

		impl_guard.release();
	}

	Display::~Display()
	{
		try
		{
			if(!UnregisterClassW(reinterpret_cast<LPCWSTR>(impl->classatom), impl->hInstance))
			{
				std::cerr << "Can't unregister win32 window class: " << win32::getErrorMsg();
			}
		} catch(...)
		{
		}

		delete impl;
	}

	void Display::wait()
	{
		MSG msg;

		if(GetMessageW(&msg, 0, 0, 0) == -1) // Yep, the return type is BOOL and it returns -1 on error
			throw Exception("Can't get win32 message: " + win32::getErrorMsg());

		TranslateMessage(&msg);

		if(DispatchMessageW(&msg) < 0)
			throw Exception("Can't dispatch message to window procedures: " + win32::getErrorMsg());

		poll();
	}

	void Display::poll()
	{
		MSG msg;

		while(PeekMessageW(&msg, 0, 0, 0, PM_REMOVE)) // NOTE: PeekMessageW does not return errors
		{
			TranslateMessage(&msg);
			if(DispatchMessageW(&msg) < 0)
				throw Exception("Can't dispatch message to window procedures: " + win32::getErrorMsg());
		}
	}
}
