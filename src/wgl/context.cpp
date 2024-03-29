#include <iostream>
#include <memory>

#include <windows.h>

#include <wm/exception.hpp>
#include <wm/display.hpp>
#include <wm/window.hpp>
#include <wm/pixelformat.hpp>
#include <wm/context.hpp>
#include <wm/surface.hpp>
#include <wm/configuration.hpp>
#include <wm/pixelformat.hpp>

#include <win32/impl/error.hpp>
#include <win32/impl/display_impl.hpp>
#include <win32/impl/window_impl.hpp>
#include <wgl/impl/surface_impl.hpp>
#include <wgl/impl/pixelformat_impl.hpp>
#include <wgl/impl/configuration_impl.hpp>
#include <wgl/impl/dummywindow.hpp>
#include <wgl/impl/extensions.hpp>

#include <wm/export.hpp>

namespace
{
	void makeCurrent(const wm::wgl::Extensions& ext, HGLRC hglrc, HDC drawdc, HDC readdc)
	{
		if(ext.ARB_make_current_read)
		{
			if(!ext.wglMakeContextCurrentARB(drawdc, readdc, hglrc))
			{
				DWORD err = GetLastError();
				std::string msg;
				if(err == ERROR_INVALID_PIXEL_TYPE_ARB) msg = "Invalid pixel type";
				else if(err == ERROR_INCOMPATIBLE_DEVICE_CONTEXTS_ARB) msg = "Incompatible device contexts";
				else msg = wm::win32::getErrorMsg(err);
				throw wm::Exception("Can't set current context: " + msg);
			}
		} else
		{
			if(drawdc != readdc)
				throw wm::Exception("Separate draw and read surfaces not supported");

			if(!wglMakeCurrent(drawdc, hglrc))
				throw wm::Exception("Can't set current context: " + wm::win32::getErrorMsg());
		}
	}

	void getCurrent(const wm::wgl::Extensions& ext, HGLRC& hglrc, HDC& drawdc, HDC& readdc)
	{
		hglrc = wglGetCurrentContext();
		drawdc = wglGetCurrentDC();

		if(ext.ARB_make_current_read)
		{
			// this if here protects from a bug in Nvidia drivers (ver 190.89/WinXP 32bit)
			if(hglrc) readdc = ext.wglGetCurrentReadDCARB();
			else readdc = 0;
		} else
		{
			readdc = drawdc;
		}
	}
}

namespace wm
{
	struct Context::impl_t
	{
		explicit impl_t(wm::Configuration &config)
			: config(config)
		{
		}

		wm::Configuration &config;
		HGLRC hglrc;
	};

	Context::Context(
		const PixelFormat &format,
		int versionMajor,
		int versionMinor,
		bool compatible,
		bool debug,
		bool,
		Context *shared)
		: impl(new impl_t(format.configuration()))
		, display_(&format.configuration().display())
	{
		std::auto_ptr<impl_t> impl_guard(impl); // deletes impl object in case of exception

		const wm::wgl::Extensions &extensions = impl->config.impl->extensions;

		{
			wgl::DummyWindow dummywin(display().impl->hInstance);
			wgl::DCGetter getter(dummywin.hwnd);

			// TODO: check dummy window pixel format compatibility here

			PIXELFORMATDESCRIPTOR pfd;
			if(!SetPixelFormat(getter.hdc, format.impl->index, &pfd))
				throw wm::Exception("Can't set pixel format: " + wm::win32::getErrorMsg());

			if(extensions.ARB_create_context)
			{
				// NOTE: WGL_ARB_CONTEXT_PROFILE_MASK is valid only if requested version >= 3.2
				bool useProfile = extensions.ARB_create_context_profile
					&& (versionMajor > 3 || (versionMajor == 3 && versionMinor >= 2));

				int attribs[] = {
					WGL_CONTEXT_MAJOR_VERSION_ARB, versionMajor,
					WGL_CONTEXT_MINOR_VERSION_ARB, versionMinor,
					WGL_CONTEXT_FLAGS_ARB, 0
						| (debug ? WGL_CONTEXT_DEBUG_BIT_ARB : 0)
						| (compatible ? 0 : WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB),
					(useProfile ? WGL_CONTEXT_PROFILE_MASK_ARB : 0),
						(compatible ? WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB :
							WGL_CONTEXT_CORE_PROFILE_BIT_ARB),
					0 };

				impl->hglrc = extensions.wglCreateContextAttribsARB(
					getter.hdc,
					(shared ? shared->impl->hglrc : 0),
					attribs);

				if(!impl->hglrc)
				{
					DWORD err = GetLastError();
					std::string msg;
					if(err == ERROR_INVALID_VERSION_ARB) msg = "Invalid version";
					else if(err == ERROR_INVALID_PROFILE_ARB) msg = "Invalid profile";
					else msg = win32::getErrorMsg(err);
					throw wm::Exception("Can't create context: " + msg);
				}
			} else
			{
				impl->hglrc = wglCreateContext(getter.hdc);
				if(!impl->hglrc)
					throw wm::Exception("Can't create Context" + wm::win32::getErrorMsg());
			}
		}

		if(shared && !extensions.ARB_create_context)
		{
			if(!wglShareLists(shared->impl->hglrc, impl->hglrc))
			{
				DWORD err = GetLastError();
				wglDeleteContext(impl->hglrc);
				throw Exception("Can't share contexts" + win32::getErrorMsg(err));
			}
		}

		impl_guard.release();
	}

	Context::~Context()
	{
		try
		{
			if(!wglDeleteContext(impl->hglrc))
				std::cerr << "Can't delete Context: " << win32::getErrorMsg() << std::endl;
		} catch(...)
		{
		}

		delete impl;
	}

	bool Context::direct() const { return true; }

	CurrentContext::CurrentContext(Context &context, Surface &draw, Surface &read, bool do_setup)
		: context(context)
		, draw(draw)
		, read(read)
		, do_reset(false)
		, num1(0), num2(0), num3(0)
		, ptr1(0), ptr2(0), ptr3(0)
	{
		if(do_setup) setup();
	}

	CurrentContext::CurrentContext(Context &context, Surface &drawread, bool do_setup)
		: context(context)
		, draw(drawread)
		, read(drawread)
		, do_reset(false)
		, num1(0), num2(0), num3(0)
		, ptr1(0), ptr2(0), ptr3(0)
	{
		if(do_setup) setup();
	}
	
	CurrentContext::~CurrentContext()
	{
		try
		{
			if(do_reset) reset();
		} catch(wm::Exception& exc)
		{
			std::cerr << exc.what() << std::endl;
		}
	}

	void CurrentContext::setup()
	{
		const wm::wgl::Extensions &ext = context.impl->config.impl->extensions;

		HGLRC oldctx;
		HDC olddrawdc, oldreaddc;

		getCurrent(ext, oldctx, olddrawdc, oldreaddc);

		HDC drawdc, readdc;
		HGLRC hglrc = context.impl->hglrc;

		{
			wgl::DCGetter getter(draw.impl->hwnd);
			drawdc = getter.hdc;

			if(&read != &draw)
			{
				wgl::DCGetter readget(read.impl->hwnd);
				readdc = readget.hdc;
				makeCurrent(ext, hglrc, drawdc, readdc);
			} else
			{
				readdc = drawdc;
				makeCurrent(ext, hglrc, drawdc, drawdc);
			}
		}

		if(oldctx != hglrc || olddrawdc != drawdc || oldreaddc != readdc)
		{
			ptr1 = hglrc;
			ptr2 = drawdc;
			ptr3 = readdc;
		}

		do_reset = true;
	}

	void CurrentContext::reset()
	{
		const wm::wgl::Extensions &ext = context.impl->config.impl->extensions;

		makeCurrent(ext, reinterpret_cast<HGLRC>(ptr1), reinterpret_cast<HDC>(ptr2), reinterpret_cast<HDC>(ptr3));
		do_reset = false;
	}

	void CurrentContext::release()
	{
		do_reset = false;
	}

	void CurrentContext::swapInterval(wm::Configuration& config, int interval)
	{
		const wgl::Extensions &ext = config.impl->extensions;

		if(!ext.EXT_swap_control)
			throw Exception("Swap control not supported");

		if(!ext.wglSwapIntervalEXT(interval))
		{
			DWORD err = GetLastError();
			std::string msg;
			if(err == ERROR_INVALID_DATA) msg = "Negative swap interval";
			else if(err == ERROR_DC_NOT_FOUND) msg = "No current rendering context";
			else msg = win32::getErrorMsg(err);
			throw Exception("Can't set swap interval: " + msg);
		}
	}

	void CurrentContext::swapInterval(int interval)
	{
		swapInterval(context.impl->config, interval);
	}
}
