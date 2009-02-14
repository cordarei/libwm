#include <windows.h>

#include <wm/exception.hpp>
#include <wm/pixelformat.hpp>
#include <wm/context.hpp>
#include <wm/window.hpp>

#include "impl/window_impl.hpp"

namespace
{
	HGLRC createContext(HWND hwnd, const wm::PixelFormat &format)
	{
		HDC hdc = GetDC(hwnd);

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
			ReleaseDC(hwnd, hdc);
			throw wm::Exception("Can't choose pixel format");
		}
		
		if(!SetPixelFormat(hdc, fmt, &pfd))
		{
			ReleaseDC(hwnd, hdc);
			throw wm::Exception("Can't set pixel format");
		}

		HGLRC hglrc = wglCreateContext(hdc);
		if(!hglrc)
		{
			ReleaseDC(hwnd, hdc);
			throw wm::Exception("Can't create Context");
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
			wglDeleteContext(impl->hglrc);
			throw Exception("Can't share contexts");
		}
	}
	
	Context::~Context()
	{
		wglDeleteContext(impl->hglrc);
	}

	void makeCurrent(Window &window, Context &context)
	{
		HDC hdc = GetDC(window.impl->hwnd);

		if(!wglMakeCurrent(hdc, context.impl->hglrc))
		{
			ReleaseDC(window.impl->hwnd, hdc);	
			throw Exception("Can't set current context");
		}

		ReleaseDC(window.impl->hwnd, hdc);	
	}
}
