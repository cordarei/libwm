#include <cstdint>
#include <map>

#include <wm/exception.hpp>
#include <wm/window.hpp>
#include <wm/event.hpp>
#include <wm/keyboard.hpp>

#include <win32/impl/keymap.hpp>
#include <win32/impl/eventreader.hpp>
#include <win32/impl/window_impl.hpp>
#include <win32/impl/error.hpp>

#include <windows.h>
#undef MOD_SHIFT
#undef MOD_CONTROL

namespace
{
    uint32_t wparam2unicode(WPARAM wparam)
    {
        uint16_t lo = wparam & 0xFFFF;
        uint16_t hi = (wparam & 0xFFFF0000) >> 16;
        uint32_t codepoint = 0;

        // Convert from UTF-16 to Unicode codepoint
        if(lo < 0xD8000 || lo > 0xDFFF) codepoint = lo;
        else if(lo < 0xD800 || lo > 0xDBFF) throw wm::Exception("Win32 WM_CHAR message contains invalid UTF-16");
        else if(hi < 0xDC00 || hi > 0xDFFF) throw wm::Exception("Win32 WM_CHAR message contains invalid UTF-16");
        else codepoint = ((lo & 0x3FF) << 10) | (hi & 0x3FF) + 0x10000;

        return codepoint;
    }

    bool fromWin32Event(
            wm::Window& window,
            UINT message,
            WPARAM wparam,
            LPARAM lparam,
            wm::Event &event)
    {
        switch(message)
        {
            case WM_CHAR:
                event.text.type = wm::TEXT_INPUT;
                event.text.window = &window;
                event.text.unicode = wparam2unicode(wparam);
                return true;

            case WM_LBUTTONDOWN:
            case WM_LBUTTONUP:
            case WM_MBUTTONDOWN:
            case WM_MBUTTONUP:
            case WM_RBUTTONDOWN:
            case WM_RBUTTONUP:
            case WM_XBUTTONDOWN:
            case WM_XBUTTONUP:
            case WM_MOUSEWHEEL:
            case WM_MOUSEHWHEEL:
                event.button.type = wm::win32::mapButtonState(message) ? wm::BUTTON_DOWN : wm::BUTTON_UP;
                event.button.window = &window;
                event.button.x = LOWORD(lparam);
                event.button.y = HIWORD(lparam);
                event.button.button = wm::win32::mapButton(message, wparam);
                event.button.buttons = wm::win32::mapButtons(wparam);
                event.button.keymod = wm::win32::mapKeyMod(wparam)
                    | (wm::win32::getKeyModState() &
                        ~(wm::keyboard::MOD_SHIFT |
                        wm::keyboard::MOD_CONTROL));
                return true;

            case WM_SETFOCUS:
            case WM_KILLFOCUS:
                event.focus.type = (message == WM_SETFOCUS) ? wm::FOCUS_GOT : wm::FOCUS_LOST;
                event.focus.window = &window;
                return true;

            case WM_CLOSE:
                event.close.type = wm::CLOSE;
                event.close.window = &window;
                return true;

            default:
                return false;
        }
    }
}


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

        return reader.handleEvent(window, hwnd, message, wparam, lparam);
    }

    LRESULT EventReader::handleEvent(
        Window& window,
        HWND hwnd,
        UINT message,
        WPARAM wparam,
        LPARAM lparam)
    {
        switch(message)
        {
            case WM_ENTERSIZEMOVE: return handleEnterSizeMove(window, hwnd, message, wparam, lparam);
            case WM_EXITSIZEMOVE: return handleExitSizeMove(window, hwnd, message, wparam, lparam);
            case WM_SIZE: return handleSize(window, hwnd, message, wparam, lparam);
            case WM_PAINT: return handlePaint(window, hwnd, message, wparam, lparam);
            case WM_ERASEBKGND: return handleEraseBkgnd(window, hwnd, message, wparam, lparam);
            case WM_KEYDOWN:
            case WM_KEYUP:
            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP: return handleKey(window, hwnd, message, wparam, lparam);
            case WM_MOUSEMOVE: return handleMotion(window, hwnd, message, wparam, lparam);
            case WM_MOUSELEAVE: return handleLeave(window, hwnd, message, wparam, lparam);
            case WM_GETMINMAXINFO: return handleGetMinMaxInfo(window, hwnd, message, wparam, lparam);
            default:
                Event event;
                if(!fromWin32Event(window, message, wparam, lparam, event))
                    return DefWindowProc(hwnd, message, wparam, lparam);

                window.impl->event_queue->push(event);
                return TRUE;
        }
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
            Event event;
            event.resize.type = wm::RESIZE;
            event.resize.window = &window;
            event.resize.width = width;
            event.resize.height = height;
            window.impl->event_queue->push(event);
        }

        if(dirty)
        {
            // propagate ExposeEvent
            Event event;
            event.expose.type = wm::EXPOSE;
            event.expose.window = &window;
            event.expose.x = 0;
            event.expose.y = 0;
            event.expose.width = width;
            event.expose.height = height;
            window.impl->event_queue->push(event);
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
            // propagate ResizeEvent
            Event event;
            event.resize.type = wm::RESIZE;
            event.resize.window = &window;
            event.resize.width = w;
            event.resize.height = h;
            window.impl->event_queue->push(event);
        }

        return 0; // should return 0 if WM_SIZE processed
    }

    LRESULT EventReader::handlePaint(Window& window, HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
    {
        // ValidateRect prevents Windows from resending WM_PAINT
        RECT rect, *ptr = 0;
        if(GetUpdateRect(hwnd, &rect, FALSE))
        {
            if(!ValidateRect(hwnd, &rect)) // if ptr == NULL, validates entire window
                throw wm::Exception("Can't validate win32 window rectangle" + wm::win32::getErrorMsg());

            ptr = &rect;
        } else
        {
            DWORD err = GetLastError();
            if(err)
                throw wm::Exception("Can't get dirty rectangle, GetUpdateRect failed: " + win32::getErrorMsg(err));
        }

        if(sizemove)
        {
            dirty = true;
        } else
        {
            // propagate ExposeEvent
            Event event;
            event.expose.type = wm::EXPOSE;
            event.expose.window = &window;
            event.expose.x = ptr ? rect.left : 0;
            event.expose.y = ptr ? rect.top : 0;
            event.expose.width = ptr ? (rect.right - rect.left) : width;
            event.expose.height = ptr ? (rect.bottom - rect.top) : height;
            window.impl->event_queue->push(event);
        }

        return DefWindowProc(hwnd, message, wparam, lparam); // should return 0 if WM_PAINT processed
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

            Event event;
            event.key.type = state ? wm::KEY_DOWN : wm::KEY_UP;
            event.key.window = &window;
            event.key.symbol = translated;
            event.key.keymod = win32::getKeyModState();
            event.key.repeat = repeat;

            window.impl->event_queue->push(event);
        }

        return 0;
    }

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

            // Propagate MouseOver event
            Event event;
            event.mouseover.type = wm::MOUSE_ENTER;
            event.mouseover.window = &window;
            event.mouseover.x = x;
            event.mouseover.y = y;
            window.impl->event_queue->push(event);
        }

        wm::keyboard::KeyMod keymod = wm::win32::mapKeyMod(wparam)
            | (wm::win32::getKeyModState() &
                ~(wm::keyboard::MOD_SHIFT | wm::keyboard::MOD_CONTROL));

        Event event;
        event.motion.type = wm::MOTION;
        event.motion.window = &window;
        event.motion.x = x;
        event.motion.y = y;
        event.motion.buttons = wm::win32::mapButtons(wparam);
        event.motion.keymod = keymod;
        window.impl->event_queue->push(event);
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
            // Propagate MouseOver event
            Event event;
            event.mouseover.type = wm::MOUSE_LEAVE;
            event.mouseover.window = &window;
            event.mouseover.x = 0;
            event.mouseover.y = 0;
            window.impl->event_queue->push(event);
        }

        return 0;
    }

    LRESULT EventReader::handleGetMinMaxInfo(Window& window, HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
    {
        MINMAXINFO *ptr = reinterpret_cast<MINMAXINFO*>(lparam);

        // TODO: synchronize this!
        // this read should be atomic
        unsigned int bounds[][2] = { { window.impl->minW, window.impl->minH }, { window.impl->maxW, window.impl->maxH } };

        LONG style = GetWindowLong(hwnd, GWL_STYLE);
        if(!style)
            throw wm::Exception("Can't reply to WM_GETMINMAXINFO, can't get window style: " + win32::getErrorMsg());

        LONG exstyle = GetWindowLong(hwnd, GWL_EXSTYLE);
        if(!exstyle)
            throw wm::Exception("Can't reply to WM_GETMINMAXINFO, can't get extended window style: " + win32::getErrorMsg());

        for(int i = 0; i < 2; ++i)
        {
            if(!bounds[i][0] || !bounds[i][1]) continue;

            RECT rect;
            if(!SetRect(&rect, 0, 0, bounds[i][0], bounds[i][1]))
                throw wm::Exception("Can't set fullscreen, SetRect failed: " + win32::getErrorMsg());

            if(!AdjustWindowRectEx(&rect, style, false, exstyle))
                throw wm::Exception("Can't set fullscreen, AdjustWindowRectEx failed: " + win32::getErrorMsg());

            bounds[i][0] = rect.right - rect.left;
            bounds[i][1] = rect.bottom - rect.top;
        }

        unsigned int minW = bounds[0][0], minH = bounds[0][1], maxW = bounds[1][0], maxH = bounds[1][1];

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
