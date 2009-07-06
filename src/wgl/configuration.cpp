#include <windows.h>

#include <wm/display.hpp>
#include <wm/window.hpp>
#include <wm/configuration.hpp>

#include <win32/impl/error.hpp>
#include <win32/impl/display_impl.hpp>
#include <win32/impl/window_impl.hpp>
#include <wgl/impl/configuration_impl.hpp>
#include <wgl/impl/dummywindow.hpp>

#include <wgl/impl/extensions.hpp>

namespace wm
{
	Configuration::Configuration(Window& window)
		: impl(new impl_t)
		, display_(&window.display())
	{
		impl->extensions.init(window.display().impl->hInstance);

		HWND hwnd = window.impl->hwnd;
		HDC hdc = GetDC(hwnd);
		if(!hdc)
			throw wm::Exception("Can't get win32 device context" + wm::win32::getErrorMsg());

		int max_index = DescribePixelFormat(hdc, 1, sizeof(PIXELFORMATDESCRIPTOR), 0);

		if(max_index == 0)
		{
			DWORD err = GetLastError();
			ReleaseDC(hwnd, hdc);
			throw wm::Exception("Can't create Context" + wm::win32::getErrorMsg(err));
		}

		for(int index = 1; index <= max_index; ++index)
		{
			PIXELFORMATDESCRIPTOR pfd;

			if(!DescribePixelFormat(hdc, index, sizeof(PIXELFORMATDESCRIPTOR), &pfd))
			{
				DWORD err = GetLastError();
				ReleaseDC(hwnd, hdc);
				throw wm::Exception("Can't create Context" + wm::win32::getErrorMsg(err));
			}

			if(!(pfd.dwFlags & PFD_SUPPORT_OPENGL) ||
				!(pfd.dwFlags & PFD_DRAW_TO_WINDOW) ||
				(pfd.dwFlags & PFD_NEED_PALETTE) ||
				(pfd.dwFlags & PFD_NEED_SYSTEM_PALETTE) ||
				!(pfd.dwFlags & PFD_DOUBLEBUFFER) ||
				pfd.iPixelType != PFD_TYPE_RGBA)
				continue;

			int red, green, blue, alpha;
			int depth, stencil;

			red = pfd.cRedBits;
			green = pfd.cGreenBits;
			blue = pfd.cBlueBits;
			alpha = pfd.cAlphaBits;

			depth = pfd.cDepthBits;
			stencil = pfd.cStencilBits;

			impl->formatdata.push_back(PixelFormat::impl_t(index));
			impl->formats.push_back(
				PixelFormat(
					PixelFormat::Descriptor(red, green, blue, alpha, depth, stencil),
					*this,
					impl->formatdata.back()
					));
		}

		ReleaseDC(hwnd, hdc);
	}
	
	Configuration::~Configuration()
	{
		delete impl;
	}
	
	int Configuration::numFormats() const { return impl->formats.size(); }
	const PixelFormat& Configuration::getFormat(int index) const { return impl->formats.at(index); }
}