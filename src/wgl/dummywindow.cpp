#include <windows.h>

#include <cstdlib>
#include <iostream>

#include <wm/exception.hpp>

#include <win32/impl/error.hpp>
#include <win32/impl/classname.hpp>
#include <wgl/impl/dummywindow.hpp>

namespace
{
	static LRESULT CALLBACK wndproc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
	{
		return DefWindowProc(hwnd, message, wparam, lparam);
	}
}

namespace wm
{
	namespace wgl
	{
		DummyWindow::DummyWindow(HINSTANCE hinstance)
		{
			this->hinstance = hinstance;

			WCHAR classname[sizeof(DummyWindow*) * 2 + 1];
			win32::genClassNameStr(this, classname);

			WNDCLASSEXW klass;
			klass.cbSize = sizeof(WNDCLASSEXW);
			klass.style = CS_OWNDC;
			klass.lpfnWndProc = &wndproc;
			klass.cbClsExtra = 0;
			klass.cbWndExtra = 0;
			klass.hInstance = hinstance;
			klass.hIcon = 0;
			klass.hCursor = 0;
			klass.hbrBackground = 0;
			klass.lpszMenuName = 0;
			klass.lpszClassName = &classname[0];
			klass.hIconSm = 0;

			classatom = RegisterClassExW(&klass);
			if(!classatom)
				throw Exception("Can't register win32 window class: " + win32::getErrorMsg());

			hwnd = CreateWindowExW(
				0,
				&classname[0],
				L"",	// window title
				WS_POPUP | WS_DISABLED,
				CW_USEDEFAULT, CW_USEDEFAULT,
				100, 100, // width, height
				0,
				0,
				hinstance,
				0);

			if(!hwnd)
			{
				DWORD err = GetLastError();
				UnregisterClassW(&classname[0], hinstance);
				throw wm::Exception("Can't create Window: " + wm::win32::getErrorMsg(err));
			}
		}

		DummyWindow::~DummyWindow()
		{
			if(!DestroyWindow(hwnd))
			{
				std::cerr << "Can't destroy window" << win32::getErrorMsg() << std::endl;
			}

			if(!UnregisterClassW(reinterpret_cast<LPCWSTR>(classatom), hinstance))
			{
				std::cerr << "Can't unregister win32 window class: " << win32::getErrorMsg();
			}
		}

		DummyContext::DummyContext(HWND hwnd)
		{
			PIXELFORMATDESCRIPTOR pfd;
			std::memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
			pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
			pfd.nVersion = 1;
			pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
			pfd.iPixelType = PFD_TYPE_RGBA;
			pfd.iLayerType = PFD_MAIN_PLANE;

			DCGetter getter(hwnd);

			int pixelformat = ChoosePixelFormat(getter.hdc, &pfd);
			if(!pixelformat)
				throw wm::Exception("ChoosePixelFormat failed: " + win32::getErrorMsg());

			if(!SetPixelFormat(getter.hdc, pixelformat, &pfd))
				throw wm::Exception("SetPixelFormat failed: " + win32::getErrorMsg());

			hglrc = wglCreateContext(getter.hdc);
			if(!hglrc)
				throw wm::Exception("Can't create Context" + wm::win32::getErrorMsg());
		}

		DummyContext::~DummyContext()
		{
			if(!wglDeleteContext(hglrc))
				std::cerr << "Can't delete Context: " << win32::getErrorMsg() << std::endl;
		}

		UseContext::UseContext(HWND hwnd, HGLRC hglrc)
		{
			oldContext = wglGetCurrentContext();
			oldDC = wglGetCurrentDC();

			DCGetter getter(hwnd);

			if(!wglMakeCurrent(getter.hdc, hglrc))
				throw Exception("Can't set current context: " + win32::getErrorMsg());
		}

		UseContext::~UseContext()
		{
			if(!wglMakeCurrent(oldDC, oldContext))
			{
				std::cerr << "Can't set current context: " << win32::getErrorMsg() << std::endl;
			}
		}

		DCGetter::DCGetter(HWND hwnd)
			: hdc(0)
			, hwnd(hwnd)
		{
			hdc = GetDC(hwnd);
			if(!hdc)
				throw wm::Exception("Can't get win32 device context: " + win32::getErrorMsg());
		}

		DCGetter::~DCGetter()
		{
			if(!ReleaseDC(hwnd, hdc))
				std::cerr << "Device context not released!" << std::endl;
		}
	}
}
