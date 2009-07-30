#ifndef WM_WIN32_IMPL_EVENTREADER_HPP
#define WM_WIN32_IMPL_EVENTREADER_HPP

#include <windows.h>

namespace wm
{
	class Window;

	class EventReader
	{
		public:
			EventReader()
				: sizemove(false)
				, resizing(false)
				, dirty(false)
				, mouseinitialized(false)
				, mouseinside(false)
				, mousehidden(false)
			{
			}

			static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

		private:
			bool handleEvent(Window&, HWND, UINT, WPARAM, LPARAM, LRESULT&);

			LRESULT EventReader::handleEnterSizeMove(Window&, HWND, UINT, WPARAM, LPARAM);
			LRESULT EventReader::handleExitSizeMove(Window&, HWND, UINT, WPARAM, LPARAM);
			LRESULT EventReader::handleSize(Window&, HWND, UINT, WPARAM, LPARAM);
			LRESULT EventReader::handlePaint(Window&, HWND, UINT, WPARAM, LPARAM);
			LRESULT EventReader::handleEraseBkgnd(Window&, HWND, UINT, WPARAM, LPARAM);
			LRESULT EventReader::handleKey(Window&, HWND, UINT, WPARAM, LPARAM);
			LRESULT EventReader::handleMotion(Window&, HWND, UINT, WPARAM, LPARAM);
			LRESULT EventReader::handleLeave(Window&, HWND, UINT, WPARAM, LPARAM);
			LRESULT handleGetMinMaxInfo(Window& window, HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

			bool sizemove, resizing, dirty;	// win32 size-move state
			unsigned int width, height;

			bool mouseinitialized, mouseinside;
			bool mousehidden;

			friend class Window;
	};
}

#endif