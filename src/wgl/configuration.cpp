#include <windows.h>

#include <wm/window.hpp>
#include <wm/configuration.hpp>

#include <win32/impl/error.hpp>
#include <win32/impl/window_impl.hpp>
#include <wgl/impl/configuration_impl.hpp>

namespace wm
{
	Configuration::Configuration(Window& window)
		: impl(new impl_t)
		, display_(&window.display())
	{
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
				pfd.iPixelType != PFD_TYPE_RGBA)
				continue;

			impl->formatdata.push_back(PixelFormat::impl_t());
			PixelFormat::impl_t &format = impl->formatdata.back();

			format.index = index;

			format.red = pfd.cRedBits;
			format.green = pfd.cGreenBits;
			format.blue = pfd.cBlueBits;
			format.alpha = pfd.cAlphaBits;

			format.depth = pfd.cDepthBits;
			format.stencil = pfd.cStencilBits;
		}

		ReleaseDC(hwnd, hdc);
	}
	
	Configuration::~Configuration()
	{
		delete impl;
	}
	
	int Configuration::numFormats() const { return impl->formatdata.size(); }

	PixelFormat Configuration::getFormat(int index) const { return PixelFormat(*this, &impl->formatdata.at(index)); }
}