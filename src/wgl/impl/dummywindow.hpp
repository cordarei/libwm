#ifndef DUMMYWINDOW_HPP
#define DUMMYWINDOW_HPP

#include <vector>

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

				std::vector<WCHAR> classname;
				HINSTANCE hinstance;
		};

		struct DummyContext
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
				UseContext(HWND hwnd, HGLRC hglrc);
				~UseContext();

			private:
				HGLRC oldContext;
				HDC oldDC;

		};
	}
}

#endif
