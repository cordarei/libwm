#ifndef DUMMYWINDOW_HPP
#define DUMMYWINDOW_HPP

#include <windows.h>

namespace wm
{
	namespace wgl
	{
		class DummyWindow
		{
			public:
				explicit DummyWindow(HINSTANCE hinstance);
				~DummyWindow();

				HWND hwnd;

			private:
				DummyWindow(const DummyWindow&);
				DummyWindow& operator=(const DummyWindow&);

				ATOM classatom;
				HINSTANCE hinstance;
		};

		class DummyContext
		{
			public:
				explicit DummyContext(HWND hwnd);
				~DummyContext();

				HGLRC hglrc;

			private:
				DummyContext(const DummyContext&);
				DummyContext& operator=(const DummyContext&);
		};

		class UseContext
		{
			public:
				UseContext(HDC hdc, HGLRC hglrc);
				~UseContext();

			private:
				UseContext(const UseContext&);
				UseContext& operator=(const UseContext&);

				HGLRC oldContext;
				HDC oldDC;

		};

		class DCGetter
		{
			public:
				explicit DCGetter(HWND hwnd);
				~DCGetter();

				HDC hdc;

			private:
				DCGetter(const DCGetter&);
				DCGetter& operator=(const DCGetter&);

				HWND hwnd;
		};
	}
}

#endif
