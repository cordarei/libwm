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
	HGLRC createContext(HWND hwnd, const wm::PixelFormat &format)
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
		impl->hglrc = createContext(window.impl->hwnd, window.impl->format);
	}

	Context::Context(Window &window, Context& sharedContext, int majorVersion, int minorVersion)
		: impl(new impl_t)
		, display_(window.display())
	{
		impl->hglrc = createContext(window.impl->hwnd, window.impl->format);

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
			wglDeleteContext(impl->hglrc);
			std::cerr << "Can't delete Context: " << win32::getErrorMsg() << std::endl;
		} catch(...)
		{
		}
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
