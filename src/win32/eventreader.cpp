#include <map>

#include <wm/exception.hpp>
#include <wm/window.hpp>
#include <wm/events.hpp>

#include <win32/impl/keymap.hpp>
#include <win32/impl/eventreader.hpp>
#include <win32/impl/eventfactory.hpp>
#include <win32/impl/window_impl.hpp>
#include <win32/impl/error.hpp>

#include <windows.h>

namespace wm
{
	LRESULT CALLBACK EventReader::WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
	{
		SetLastError(0);
		wm::Window *ptr =
			reinterpret_cast<wm::Window*>(
				GetWindowLongPtr(hwnd, 0));
		if(!ptr)
		{
			DWORD err = GetLastError();
			if(err)
				throw wm::Exception("Can't get win32 window user data" +
					wm::win32::getErrorMsg(err));
			return DefWindowProc(hwnd, message, wparam, lparam);
		}

		wm::Window& window = *ptr;
		EventReader& reader = window.impl->eventreader;
		
		LRESULT result;
		if(reader.handleEvent(window, hwnd, message, wparam, lparam, result))
			return result;

		const wm::Event *event = wm::win32::fromWin32Event(window, message, wparam, lparam);
		if(event)
		{
			window.impl->eventq.push(event);
			return TRUE;
		}

		return DefWindowProc(hwnd, message, wparam, lparam);
	}

	bool EventReader::handleEvent(
		Window& window,
		HWND hwnd,
		UINT message,
		WPARAM wparam,
		LPARAM lparam,
		LRESULT &result)
	{
		typedef LRESULT (EventReader::*HandlerFunc)(Window&,HWND,UINT,WPARAM,LPARAM);

		static const struct Registry
		{
			Registry()
			{
				map[WM_ENTERSIZEMOVE] = &EventReader::handleEnterSizeMove;
				map[WM_EXITSIZEMOVE] = &EventReader::handleExitSizeMove;
				map[WM_SIZE] = &EventReader::handleSize;
				map[WM_PAINT] = &EventReader::handlePaint;
				map[WM_ERASEBKGND] = &EventReader::handleEraseBkgnd;
				map[WM_KEYDOWN] = &EventReader::handleKey;
				map[WM_KEYUP] = &EventReader::handleKey;
				map[WM_SYSKEYDOWN] = &EventReader::handleKey;
				map[WM_SYSKEYUP] = &EventReader::handleKey;
				map[WM_MOUSEMOVE] = &EventReader::handleMotion;
				map[WM_MOUSELEAVE] = &EventReader::handleLeave;
				map[WM_GETMINMAXINFO] = &EventReader::handleGetMinMaxInfo;
			}

			typedef std::map<UINT, HandlerFunc> map_t;
			map_t map;
		} registry;

		Registry::map_t::const_iterator iter = registry.map.find(message);
		if(iter == registry.map.end()) return false;

		HandlerFunc handler = iter->second;
		result = (this->*handler)(window, hwnd, message, wparam, lparam);
		return true;
	}

	LRESULT EventReader::handleEnterSizeMove(Window& window, HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
	{
		sizemove = true;
		resizing = false;
		dirty = false;
		return TRUE;
	}

	LRESULT EventReader::handleExitSizeMove(Window& window, HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
	{
		if(resizing)
		{
			// propagate ResizeEvent
			window.impl->eventq.push(
				new wm::ResizeEvent(
					window, 
					width,
					height
					));
		}

		if(dirty)
		{
			// propagate ExposeEvent
			window.impl->eventq.push(
				new wm::ExposeEvent(
					window,
					0,
					0,
					width,
					height
					));
		}

		sizemove = false;

		return TRUE;
	}

	LRESULT EventReader::handleSize(Window& window, HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
	{
		unsigned int w = LOWORD(lparam),
					h = HIWORD(lparam);

		width = w;
		height = h;

		if(sizemove) resizing = true;
		else
		{
			window.impl->eventq.push(
				new ResizeEvent(
					window,
					w,
					h));
		}

		return 0; // should return 0 if WM_SIZE processed
	}

	LRESULT EventReader::handlePaint(Window& window, HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
	{
		// ValidateRect prevents Windows from resending WM_PAINT
		RECT rect, *ptr = 0;
		if(GetUpdateRect(hwnd, &rect, FALSE)) ptr = &rect;
		if(!ValidateRect(hwnd, ptr)) // if ptr == NULL, validates entire window
			throw wm::Exception("Can't validate win32 window rectangle" + wm::win32::getErrorMsg());

		if(sizemove)
		{
			dirty = true;
			return 0; // should return 0 if WM_PAINT processed
		} else
		{
			// propagate ExposeEvent
			window.impl->eventq.push(
				new ExposeEvent(
					window,
					ptr ? rect.left : 0,
					ptr ? rect.top : 0,
					ptr ? (rect.right - rect.left) : width,
					ptr ? (rect.bottom - rect.top) : height
					));
			return 0;
		}
	}

	LRESULT EventReader::handleEraseBkgnd(Window& window, HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
	{
		if(sizemove) dirty = true;

		return TRUE;
	}

	LRESULT EventReader::handleKey(Window& window, HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
	{
		bool filter;

		wm::keyboard::Symbol translated =
			win32::translateKeyEvent(hwnd, message, wparam, lparam, filter);

		if(!filter)
		{
			bool state = (message == WM_KEYDOWN || message == WM_SYSKEYDOWN);
			bool repeat = state && ((HIWORD(lparam) & KF_REPEAT) ? true : false);

			window.impl->eventq.push(new KeyEvent(
				window,
				translated,
				win32::getKeyModState(),
				state,
				repeat));
		}

		return 0;
	}

#undef MOD_SHIFT
#undef MOD_CONTROL

	LRESULT EventReader::handleMotion(Window& window, HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
	{
		// TODO: synchronize this!
		// the critical section is between EventReader mouse stuff
		// and Window::impl_t::cursorVisible in Window::showCursor()
		if(!window.impl->cursorVisible)
		{
			if(!mouseinside || mousehidden)
			{
				ShowCursor(false);
				mousehidden = false; // a bit counterintuitive
			}
		}

		unsigned int x = LOWORD(lparam);
		unsigned int y = HIWORD(lparam);

		if(!mouseinside)
		{
			if(!mouseinitialized)
			{
				mouseinitialized = true;

			} else
			{
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof(TRACKMOUSEEVENT);
				tme.dwFlags = TME_LEAVE;
				tme.hwndTrack = hwnd;

				if(!TrackMouseEvent(&tme))
					throw wm::Exception("Can't track mouse event: " + win32::getErrorMsg());
			}
			
			mouseinside = true;
			window.impl->eventq.push(new MouseOverEvent(window, x, y, true));
		}

		wm::keyboard::KeyMod keymod = wm::win32::mapKeyMod(wparam)
			| (wm::win32::getKeyModState() &
				~(wm::keyboard::MOD_SHIFT | wm::keyboard::MOD_CONTROL));

		window.impl->eventq.push(new wm::MotionEvent(window, x, y, wm::win32::mapButtons(wparam), keymod));
		return 0;
	}

	LRESULT EventReader::handleLeave(Window& window, HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
	{
		// TODO: synchronize this!
		// the critical section is between EventReader mouse stuff
		// and Window::impl_t::cursorVisible in Window::showCursor()
		if(!window.impl->cursorVisible && !mousehidden && mouseinitialized)
		{
			ShowCursor(true);
			mousehidden = true;
		}

		mouseinside = false;

		if(!mouseinitialized)
		{
			mouseinitialized = true;
		} else
		{
			window.impl->eventq.push(
				new MouseOverEvent(window, 0, 0, false));
		}

		return 0;
	}

	LRESULT EventReader::handleGetMinMaxInfo(Window& window, HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
	{
		MINMAXINFO *ptr = reinterpret_cast<MINMAXINFO*>(lparam);

		unsigned int minW, minH, maxW, maxH;

		{
			// TODO: synchronize this!
			// this read should be atomic
			minW = window.impl->minW; minH = window.impl->minH;
			maxW = window.impl->maxW; maxH = window.impl->maxH;
		}

		LRESULT result = 0;

		if(minW != 0 && minH != 0)
		{
			ptr->ptMinTrackSize.x = minW;
			ptr->ptMinTrackSize.y = minH;
			result = TRUE;
		}

		if(maxW != 0 && maxH != 0)
		{
			ptr->ptMaxSize.x = ptr->ptMaxTrackSize.x = maxW;
			ptr->ptMaxSize.y = ptr->ptMaxTrackSize.y = maxH;
			result = TRUE;
		}

		return result;
	}

}
