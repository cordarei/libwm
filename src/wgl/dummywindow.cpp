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

			classname = win32::genClassNameStr(this);

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

			if(!RegisterClassExW(&klass))
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

			if(!UnregisterClassW(&(classname)[0], hinstance))
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

			HDC hdc = GetDC(hwnd);
			if(!hdc)
				throw wm::Exception("Can't get win32 device context" + wm::win32::getErrorMsg());
			
			int pixelformat = ChoosePixelFormat(hdc, &pfd);
			if(!pixelformat)
			{
				DWORD err = GetLastError();
				ReleaseDC(hwnd, hdc);
				throw wm::Exception("ChoosePixelFormat failed: " + win32::getErrorMsg(err));
			}

			if(!SetPixelFormat(hdc, pixelformat, &pfd))
			{
				DWORD err = GetLastError();
				ReleaseDC(hwnd, hdc);
				throw wm::Exception("SetPixelFormat failed: " + win32::getErrorMsg(err));			
			}

			hglrc = wglCreateContext(hdc);
			if(!hglrc)
			{
				DWORD err = GetLastError();
				ReleaseDC(hwnd, hdc);
				throw wm::Exception("Can't create Context" + wm::win32::getErrorMsg(err));
			}
			ReleaseDC(hwnd, hdc);
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

			HDC hdc = GetDC(hwnd);
			if(!hdc)
				throw wm::Exception("Can't get win32 device context" + wm::win32::getErrorMsg());

			if(!wglMakeCurrent(hdc, hglrc))
			{
				ReleaseDC(hwnd, hdc);	
				throw Exception("Can't set current context: " + win32::getErrorMsg());
			}

			ReleaseDC(hwnd, hdc);
		}

		UseContext::~UseContext()
		{
			if(!wglMakeCurrent(oldDC, oldContext))
			{
				std::cerr << "Can't set current context: " << win32::getErrorMsg() << std::endl;
			}
		}
	}
}
