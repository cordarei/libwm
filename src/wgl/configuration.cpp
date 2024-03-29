#include <memory>

#include <boost/scoped_ptr.hpp>

#include <windows.h>

#include <wm/display.hpp>
#include <wm/window.hpp>
#include <wm/configuration.hpp>

#include <win32/impl/error.hpp>
#include <wm/display.hpp>
#include <win32/impl/display_impl.hpp>
#include <win32/impl/window_impl.hpp>
#include <wgl/impl/configuration_impl.hpp>
#include <wgl/impl/dummywindow.hpp>
#include <wgl/impl/pixelformat_impl.hpp>

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
				pfd.cStencilBits,
				0, 0, false,
				wm::PixelFormat::INTEGER,
				(pfd.dwFlags & PFD_GENERIC_FORMAT) != 0);
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

			bool valid_pixel_type =
				(values[4] == WGL_TYPE_RGBA_ARB) ||
				(values[4] == WGL_TYPE_RGBA_FLOAT_ARB && extensions.ARB_pixel_format_float) ||
				(values[4] == WGL_TYPE_RGBA_UNSIGNED_FLOAT_EXT && extensions.EXT_pixel_format_packed_float);

			return
				false 
				|| !(values[0])								// WGL_DRAW_TO_WINDOW_ARB
				|| (values[1])								// WGL_NEED_PALETTE_ARB
				|| (values[2])								// WGL_NEED_SYSTEM_PALETTE_ARB
				|| !(values[3])								// WGL_DOUBLE_BUFFER_ARB
				|| !valid_pixel_type						// WGL_PIXEL_TYPE_ARB
				|| !(values[5])								// WGL_SUPPORT_OPENGL_ARB
				;
		}

		virtual wm::PixelFormat::Descriptor makeDescriptor(HDC hdc, int index) const
		{
			int srgb_enum = extensions.ARB_framebuffer_sRGB ?
				WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB : WGL_FRAMEBUFFER_SRGB_CAPABLE_EXT;
			bool use_srgb = extensions.ARB_framebuffer_sRGB || extensions.EXT_framebuffer_sRGB;

			const int attributes[] = {
				WGL_RED_BITS_ARB,
				WGL_GREEN_BITS_ARB,
				WGL_BLUE_BITS_ARB,
				WGL_ALPHA_BITS_ARB,
				WGL_DEPTH_BITS_ARB,
				WGL_STENCIL_BITS_ARB,
				extensions.ARB_multisample ? WGL_SAMPLES_ARB : WGL_RED_BITS_ARB, // use valid dummy attributes if no multisampling
				extensions.ARB_multisample ? WGL_SAMPLE_BUFFERS_ARB : WGL_RED_BITS_ARB,
				use_srgb ? srgb_enum : WGL_RED_BITS_ARB,
				WGL_PIXEL_TYPE_ARB,
				WGL_ACCELERATION_ARB
				};

			const int num_attributes = sizeof(attributes) / sizeof(*attributes);
			int values[sizeof(attributes) / sizeof(*attributes)];

			if(!extensions.wglGetPixelFormatAttribivARB(hdc, index, 0, num_attributes, attributes,	values))
				throw wm::Exception("wglGetPixelFormatAttribiv failed: " + wm::win32::getErrorMsg());

			wm::PixelFormat::DataType type;
			switch(values[9]) // WGL_PIXEL_TYPE_ARB
			{
				case WGL_TYPE_RGBA_ARB:
					type = wm::PixelFormat::INTEGER;
					break;
				case WGL_TYPE_RGBA_FLOAT_ARB:
					if(!extensions.ARB_pixel_format_float) throw wm::Exception("Invalid pixel type");
					type = wm::PixelFormat::FLOAT;
					break;
				case WGL_TYPE_RGBA_UNSIGNED_FLOAT_EXT:
					if(!extensions.EXT_pixel_format_packed_float) throw wm::Exception("Invalid pixel type");
					type = wm::PixelFormat::UNSIGNED_FLOAT;
					break;
				default:
					throw wm::Exception("Invalid pixel type");
			}

			return wm::PixelFormat::Descriptor(
				values[0],			// WGL_RED_BITS_ARB
				values[1],			// WGL_GREEN_BITS_ARB
				values[2],			// WGL_BLUE_BITS_ARB
				values[3],			// WGL_ALPHA_BITS_ARB
				values[4],			// WGL_DEPTH_BITS_ARB
				values[5],			// WGL_STENCIL_BITS_ARB
				extensions.ARB_multisample ? values[6] : 0,	// WGL_SAMPLES_ARB
				extensions.ARB_multisample ? values[7] : 0,  // WGL_SAMPLE_BUFFERS_ARB
				use_srgb ? (values[8] != 0) : false,		// WGL_FRAMEBUFFER_SRGB_CAPABLE_{EXT,ARB}
				type,				// WGL_PIXEL_TYPE_ARB
				values[10] != WGL_FULL_ACCELERATION_ARB		// WGL_ACCELERATION_ARB
				);
		}

		const wm::wgl::Extensions &extensions;
	};

	PixelFormatBuilder* makeFormatBuilder(const wm::wgl::Extensions &extensions)
	{
		if(extensions.ARB_pixel_format) return new ARB_pixel_format_Builder(extensions);
		return new LegacyFormatBuilder();
	}

	void initCaps(wm::Configuration::Capabilities& caps, const wm::wgl::Extensions& ext)
	{
		caps.swapControl = ext.EXT_swap_control;
	}
}

namespace wm
{
	Configuration::Configuration(Display &display)
		: impl(new impl_t)
		, display_(&display)
	{
		std::auto_ptr<impl_t> impl_guard(impl); // deletes impl object in case of exception

		HINSTANCE hinstance	= display.impl->hInstance;

		wgl::DummyWindow dummywin(hinstance);
		wgl::DummyContext dummyctx(dummywin.hwnd);

		impl->extensions.init(dummywin, dummyctx);
		initCaps(caps_, impl->extensions);

		boost::scoped_ptr<PixelFormatBuilder> builder(makeFormatBuilder(impl->extensions));

		wgl::DCGetter getter(dummywin.hwnd);

		int max_index = builder->maxFormat(getter.hdc);

		for(int index = 1; index <= max_index; ++index)
		{
			if(builder->filterFormat(getter.hdc, index))
				continue;

			impl->formatdata.push_back(PixelFormat::impl_t(index));
			impl->formats.push_back(
				PixelFormat(
					builder->makeDescriptor(getter.hdc, index),
					*this,
					impl->formatdata.back()
					));
		}

		impl_guard.release();
	}
	
	Configuration::~Configuration()
	{
		delete impl;
	}
	
	int Configuration::numFormats() const { return impl->formats.size(); }
	const PixelFormat& Configuration::getFormat(int index) const { return impl->formats.at(index); }

	void *Configuration::getProcAddress(const char *name)
	{
		PROC proc = wglGetProcAddress(name);
		if(!proc)
			throw wm::Exception("Can't load extension function " + (name + (": " + win32::getErrorMsg())));
		return *reinterpret_cast<void**>(&proc);
	}

	void PixelFormat::set(Window& window) const
	{
		PIXELFORMATDESCRIPTOR pfd;
		wgl::DCGetter getter(window.impl->hwnd);
		if(!SetPixelFormat(getter.hdc, impl->index, &pfd))
			throw wm::Exception("Can't set window pixel format: " + win32::getErrorMsg());
	}
}
