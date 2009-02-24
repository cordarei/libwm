#include <iostream>

#include <windows.h>

#include <wm/exception.hpp>
#include <wm/pixelformat.hpp>
#include <wm/context.hpp>
#include <wm/window.hpp>

#include "impl/error.hpp"
#include "impl/window_impl.hpp"

namespace
{
	HGLRC createContext(HWND hwnd)
	{
		HDC hdc = GetDC(hwnd);
		if(!hdc)
			throw wm::Exception("Can't get win32 device context" + wm::win32::getErrorMsg());

		HGLRC hglrc = wglCreateContext(hdc);
		if(!hglrc)
		{
			DWORD err = GetLastError();
			ReleaseDC(hwnd, hdc);
			throw wm::Exception("Can't create Context" + wm::win32::getErrorMsg(err));
		}

		ReleaseDC(hwnd, hdc);
		return hglrc;
	}
}

namespace wm
{
	struct Context::impl_t
	{
		HGLRC hglrc;
	};

	Context::Context(Window &window, int majorVersion, int minorVersion)
		: impl(new impl_t)
		, display_(window.display())
	{
		impl->hglrc = createContext(window.impl->hwnd);
	}

	Context::Context(Window &window, Context& sharedContext, int majorVersion, int minorVersion)
		: impl(new impl_t)
		, display_(window.display())
	{
		impl->hglrc = createContext(window.impl->hwnd);

		if(!wglShareLists(sharedContext.impl->hglrc, impl->hglrc))
		{
			DWORD err = GetLastError();
			wglDeleteContext(impl->hglrc);
			throw Exception("Can't share contexts" + win32::getErrorMsg(err));
		}
	}
	
	Context::~Context()
	{
		try
		{
			if(wglDeleteContext(impl->hglrc)) return;
		} catch(...)
		{
		}

		std::cerr << "Can't delete Context: " << win32::getErrorMsg() << std::endl;
	}

	void makeCurrent(Window &window, Context &context)
	{
		HDC hdc = GetDC(window.impl->hwnd);
		if(!hdc)
			throw wm::Exception("Can't get win32 device context" + win32::getErrorMsg());

		if(!wglMakeCurrent(hdc, context.impl->hglrc))
		{
			ReleaseDC(window.impl->hwnd, hdc);	
			throw Exception("Can't set current context");
		}

		ReleaseDC(window.impl->hwnd, hdc);	
	}
}
