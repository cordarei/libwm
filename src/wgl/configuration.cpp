#include <boost/scoped_ptr.hpp>

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

namespace
{
	struct PixelFormatBuilder
	{
		virtual ~PixelFormatBuilder() { }

		virtual int maxFormat(HDC hdc) const = 0;
		virtual bool filterFormat(HDC hdc, int index) const = 0;
		virtual wm::PixelFormat::Descriptor makeDescriptor(HDC hdc, int index) const = 0;
	};

	struct LegacyFormatBuilder : public PixelFormatBuilder
	{
		virtual int maxFormat(HDC hdc) const
		{
			int max_index = DescribePixelFormat(hdc, 1, sizeof(PIXELFORMATDESCRIPTOR), 0);
			if(max_index == 0)
				throw wm::Exception("DescribePixelFormat failed: " + wm::win32::getErrorMsg());

			return max_index;
		}

		virtual bool filterFormat(HDC hdc, int index) const
		{
			PIXELFORMATDESCRIPTOR pfd;

			if(!DescribePixelFormat(hdc, index, sizeof(PIXELFORMATDESCRIPTOR), &pfd))
				throw wm::Exception("DescribePixelFormat failed: " + wm::win32::getErrorMsg());

			if(!(pfd.dwFlags & PFD_SUPPORT_OPENGL) ||
				!(pfd.dwFlags & PFD_DRAW_TO_WINDOW) ||
				(pfd.dwFlags & PFD_NEED_PALETTE) ||
				(pfd.dwFlags & PFD_NEED_SYSTEM_PALETTE) ||
				!(pfd.dwFlags & PFD_DOUBLEBUFFER) ||
				pfd.iPixelType != PFD_TYPE_RGBA)
				return true;

			return false;
		}

		virtual wm::PixelFormat::Descriptor makeDescriptor(HDC hdc, int index) const
		{
			PIXELFORMATDESCRIPTOR pfd;

			if(!DescribePixelFormat(hdc, index, sizeof(PIXELFORMATDESCRIPTOR), &pfd))
				throw wm::Exception("DescribePixelFormat failed: " + wm::win32::getErrorMsg());

			return wm::PixelFormat::Descriptor(
				pfd.cRedBits,
				pfd.cBlueBits,
				pfd.cGreenBits,
				pfd.cAlphaBits,
				pfd.cDepthBits,
				pfd.cStencilBits);
		}
	};

	struct ARB_pixel_format_Builder : public PixelFormatBuilder
	{
		explicit ARB_pixel_format_Builder(const wm::wgl::Extensions &extensions)
			: extensions(extensions)
		{
		}


		virtual int maxFormat(HDC hdc) const
		{
			const int attributes[] = { WGL_NUMBER_PIXEL_FORMATS_ARB };
			const int num_attributes = sizeof(attributes) / sizeof(*attributes);
			int values[sizeof(attributes) / sizeof(*attributes)];
			if(!extensions.wglGetPixelFormatAttribivARB(hdc, 0, 0, num_attributes, attributes,	values))
				throw wm::Exception("wglGetPixelFormatAttribiv failed: " + wm::win32::getErrorMsg());

			return values[0];
		}

		virtual bool filterFormat(HDC hdc, int index) const
		{
			const int attributes[] = {
				WGL_DRAW_TO_WINDOW_ARB,
				WGL_NEED_PALETTE_ARB,
				WGL_NEED_SYSTEM_PALETTE_ARB,
				WGL_DOUBLE_BUFFER_ARB,
				WGL_PIXEL_TYPE_ARB,
				WGL_SUPPORT_OPENGL_ARB
				};
			const int num_attributes = sizeof(attributes) / sizeof(*attributes);
			int values[sizeof(attributes) / sizeof(*attributes)];
			if(!extensions.wglGetPixelFormatAttribivARB(hdc, index, 0, num_attributes, attributes,	values))
				throw wm::Exception("wglGetPixelFormatAttribiv failed: " + wm::win32::getErrorMsg());

			return
				false 
				|| !(values[0])								// WGL_DRAW_TO_WINDOW_ARB
				|| (values[1])								// WGL_NEED_PALETTE_ARB
				|| (values[2])								// WGL_NEED_SYSTEM_PALETTE_ARB
				|| !(values[3])								// WGL_DOUBLE_BUFFER_ARB
				|| (values[4] != WGL_TYPE_RGBA_ARB)			// WGL_PIXEL_TYPE_ARB
				|| !(values[5])								// WGL_SUPPORT_OPENGL_ARB
				;
		}

		virtual wm::PixelFormat::Descriptor makeDescriptor(HDC hdc, int index) const
		{
			const int attributes[] = {
				WGL_RED_BITS_ARB,
				WGL_GREEN_BITS_ARB,
				WGL_BLUE_BITS_ARB,
				WGL_ALPHA_BITS_ARB,
				WGL_DEPTH_BITS_ARB,
				WGL_STENCIL_BITS_ARB
				};
			const int num_attributes = sizeof(attributes) / sizeof(*attributes);
			int values[sizeof(attributes) / sizeof(*attributes)];
			if(!extensions.wglGetPixelFormatAttribivARB(hdc, index, 0, num_attributes, attributes,	values))
				throw wm::Exception("wglGetPixelFormatAttribiv failed: " + wm::win32::getErrorMsg());

			return wm::PixelFormat::Descriptor(
				values[0],			// WGL_RED_BITS_ARB
				values[1],			// WGL_GREEN_BITS_ARB
				values[2],			// WGL_BLUE_BITS_ARB
				values[3],			// WGL_ALPHA_BITS_ARB
				values[4],			// WGL_DEPTH_BITS_ARB
				values[5]			// WGL_STENCIL_BITS_ARB
				);
		}

		const wm::wgl::Extensions &extensions;
	};

	PixelFormatBuilder* makeFormatBuilder(const wm::wgl::Extensions &extensions)
	{
		if(extensions.ARB_pixel_format) return new ARB_pixel_format_Builder(extensions);
		return new LegacyFormatBuilder();
	}
};

namespace wm
{
	Configuration::Configuration(Window& window)
		: impl(new impl_t)
		, display_(&window.display())
	{
		impl->extensions.init(window.display().impl->hInstance);

		boost::scoped_ptr<PixelFormatBuilder> builder(makeFormatBuilder(impl->extensions));

		HWND hwnd = window.impl->hwnd;
		HDC hdc = GetDC(hwnd);
		if(!hdc)
			throw wm::Exception("Can't get win32 device context" + wm::win32::getErrorMsg());

		try
		{
			int max_index = builder->maxFormat(hdc);

			for(int index = 1; index <= max_index; ++index)
			{
				if(builder->filterFormat(hdc, index))
					continue;

				impl->formatdata.push_back(PixelFormat::impl_t(index));
				impl->formats.push_back(
					PixelFormat(
						builder->makeDescriptor(hdc, index),
						*this,
						impl->formatdata.back()
						));
			}
		} catch(...)
		{
			ReleaseDC(hwnd, hdc);
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