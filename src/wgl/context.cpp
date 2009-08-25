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

namespace wm
{
	struct Context::impl_t
	{
		HGLRC hglrc;
		const wgl::Extensions* extensions;
	};

	Context::Context(
		const PixelFormat &format,
		int versionMajor,
		int versionMinor,
		bool compatible,
		bool debug,
		bool,
		Context *shared)
		: impl(new impl_t)
		, display_(&format.configuration().display())
	{
		std::auto_ptr<impl_t> impl_guard(impl); // deletes impl object in case of exception

		const wm::wgl::Extensions &extensions = format.configuration().impl->extensions;
		impl->extensions = &extensions;

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

	void WM_EXPORT makeCurrent(Context &context, Surface &draw, Surface &read)
	{
		const wm::wgl::Extensions &extensions = *context.impl->extensions;
		HGLRC hglrc = context.impl->hglrc;

		if(extensions.ARB_make_current_read)
		{
			wgl::DCGetter getter(draw.impl->hwnd);

			BOOL status;
			if(&draw != &read)
			{
				wgl::DCGetter readget(read.impl->hwnd);
				status = extensions.wglMakeContextCurrentARB(getter.hdc, readget.hdc, hglrc);
			} else
			{
				status = extensions.wglMakeContextCurrentARB(getter.hdc, getter.hdc, hglrc);
			}

			if(!status)
			{
				DWORD err = GetLastError();
				std::string msg;
				if(err == ERROR_INVALID_PIXEL_TYPE_ARB) msg = "Invalid pixel type";
				else if(err == ERROR_INCOMPATIBLE_DEVICE_CONTEXTS_ARB) msg = "Incompatible device contexts";
				else msg = win32::getErrorMsg(err);
				throw wm::Exception("Can't set current context: " + msg);
			}
		} else
		{
			if(&draw != &read)
				throw wm::Exception("Separate draw and read surfaces not supported");

			wgl::DCGetter getter(draw.impl->hwnd);

			if(!wglMakeCurrent(getter.hdc, hglrc))
				throw Exception("Can't set current context: " + win32::getErrorMsg());
		}
	}
}
