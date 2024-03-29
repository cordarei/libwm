#include <iostream>
#include <cstdlib>

#include <boost/scoped_array.hpp>

#include <wm/exception.hpp>
#include <wm/display.hpp>
#include <wm/window.hpp>
#include <wm/pixelformat.hpp>
#include <wm/event.hpp>

#include <win32/impl/error.hpp>
#include <win32/impl/display_impl.hpp>
#include <win32/impl/window_impl.hpp>

#include <windows.h>

namespace
{
    int getStyle(bool fullscreen, bool resizable)
    {
        return (fullscreen ? WS_POPUP : WS_OVERLAPPEDWINDOW) &
            ~(!resizable ? WS_THICKFRAME : 0);
    }

    int getExStyle(bool fullscreen)
    {
        return fullscreen ? WS_EX_APPWINDOW : WS_EX_OVERLAPPEDWINDOW;
    }

    HWND createWindow(HINSTANCE hInstance, ATOM classatom, int width, int height, int style, int exstyle)
    {
        RECT rect;
        if(!SetRect(&rect, 0, 0, width, height)) // how likely is this? ;)
            // NOTE: SetRect does not affect win32 GetLastError
            throw wm::Exception("Can't initialize window bounds rectangle");

        if(!AdjustWindowRectEx(&rect, style, false, exstyle))
            throw wm::Exception("Can't adjust window bounds rectangle" + wm::win32::getErrorMsg());

        WCHAR nullterm = 0;
        HWND hwnd = CreateWindowExW(
            exstyle,
            reinterpret_cast<LPCWSTR>(classatom),
            &nullterm,  // window title
            style,
            CW_USEDEFAULT, CW_USEDEFAULT,
            rect.right - rect.left, rect.bottom - rect.top,
            0,
            0,
            hInstance,
            0);

        if(!hwnd)
            throw wm::Exception("Can't create Window: " + wm::win32::getErrorMsg());

        return hwnd;
    }
}

namespace wm
{
    Window::Window(
        Display& display,
        unsigned int width,
        unsigned int height,
        const PixelFormat &format,
        EventQueue &event_queue)
        : impl(new impl_t)
        , display_(display)
        , pixelformat_(format)
    {
        std::auto_ptr<impl_t> impl_guard(impl); // deletes impl object in case of exception

        impl->event_queue = &event_queue;

        impl->hwnd = createWindow(
            display.impl->hInstance,
            display.impl->classatom,
            width,
            height,
            getStyle(impl->fullscreen, impl->resizable),
            getExStyle(impl->fullscreen));

        try
        {
            SetLastError(0);
            if(!SetWindowLongPtr(impl->hwnd, 0, reinterpret_cast<LONG_PTR>(this)))
            {
                DWORD err = GetLastError();
                if(err != 0)
                    throw Exception("Can't set win32 Window user data: " + win32::getErrorMsg(err));
            }

            format.set(*this);

            // TrackMouseEvent to recieve WM_MOUSELEAVE events
            TRACKMOUSEEVENT tme;
            tme.cbSize = sizeof(TRACKMOUSEEVENT);
            tme.dwFlags = TME_LEAVE;
            tme.hwndTrack = impl->hwnd;

            impl->cursorVisible = true;

            if(!TrackMouseEvent(&tme))
                throw wm::Exception("Can't track mouse events: " + win32::getErrorMsg());
        } catch(...)
        {
            if(!DestroyWindow(impl->hwnd))
            {
                std::cerr << "Can't destroy window: " << win32::getErrorMsg() << std::endl;
            }

            throw;
        }

        impl_guard.release();
    }

    Window::~Window()
    {
        try
        {
            if(!DestroyWindow(impl->hwnd))
            {
                std::cerr << "Can't destroy window: " << win32::getErrorMsg() << std::endl;
            }
        } catch(...)
        {
        }

        delete impl;
    }

    void Window::show()
    {
        ShowWindow(impl->hwnd, SW_SHOWNORMAL);
    }

    void Window::hide()
    {
        ShowWindow(impl->hwnd, SW_HIDE);
    }

    void Window::getSize(unsigned int &width, unsigned int &height)
    {
        RECT rect;
        if(!GetClientRect(impl->hwnd, &rect))
            throw wm::Exception("Can't get win32 window client area size: " + win32::getErrorMsg());

        unsigned int w = rect.right - rect.left;
        unsigned int h = rect.bottom - rect.top;

        width = w;
        height = h;
    }

    void Window::resize(unsigned int width, unsigned int height)
    {
        LONG style = GetWindowLong(impl->hwnd, GWL_STYLE);
        if(!style)
            throw wm::Exception("Can't resize window, unable to query window style: " + win32::getErrorMsg());

        LONG exstyle = GetWindowLong(impl->hwnd, GWL_EXSTYLE);
        if(!exstyle)
            throw wm::Exception("Can't resize window, unable to query extended window style: " + win32::getErrorMsg());

        RECT rect;
        if(!SetRect(&rect, 0, 0, width, height))
            throw wm::Exception("Can't set window rectangle for resizing");

        if(!AdjustWindowRectEx(&rect, style, false, exstyle))
            throw wm::Exception("Can't adjust window bounds rectangle: " + wm::win32::getErrorMsg());

        if(!SetWindowPos(
            impl->hwnd,
            0, // HWND hWndInsertAfter
            0, 0, // x, y (ignored because of SWP_NOMOVE)
            rect.right - rect.left,
            rect.bottom - rect.top,
            SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE))
            throw wm::Exception("Can't resize win32 window: " + win32::getErrorMsg());
    }

    void Window::setTitle(const char *title)
    {
        if(!title) return;

        int len = 0;
        boost::scoped_array<WCHAR> array(0);
        for(int i = 0; i < 2; ++i)
        {
            len = MultiByteToWideChar(
                CP_UTF8,
                MB_ERR_INVALID_CHARS, // fail if invalid characters
                title,
                -1, // length of input = -1  <=>  string is NULL-terminated
                array.get(), // output buffer, NULL on the first pass <=> MultiByteToWideChar calculates size of output
                len // output buffer size, in WCHAR's
                );

            if(!len)
                throw Exception("MultiByteToWideChar failed: " + win32::getErrorMsg());

            if(!array) array.reset(new WCHAR[len]);
        }

        if(!SetWindowTextW(impl->hwnd, array.get()))
            throw wm::Exception("Can't set window title: " + win32::getErrorMsg());
    }

    void Window::fullscreen(bool full)
    {
        if(full == impl->fullscreen) return;

        LONG style = getStyle(full, impl->resizable);
        LONG exstyle = getExStyle(full);

        unsigned int bounds[][2] = { { impl->minW, impl->minH }, { impl->maxW, impl->maxH } };

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

        RECT rect;
        if(full)
        {
            // Initialize windowed size for restoration
            RECT windowRect;
            if(!GetWindowRect(impl->hwnd, &windowRect))
                throw wm::Exception("Can't set fullscreen mode, GetWindowRect failed: " + win32::getErrorMsg());
            impl->windowedPosX = windowRect.left;
            impl->windowedPosY = windowRect.top;

            if(!GetClientRect(impl->hwnd, &impl->windowedRect))
                throw wm::Exception("Can't set fullscreen,  GetClientRect failed: " + win32::getErrorMsg());

            // Query full screen size
            unsigned int width, height;
            if(!(width = GetSystemMetrics(SM_CXSCREEN)) ||
                !(height = GetSystemMetrics(SM_CYSCREEN)))
                throw wm::Exception("Can't set full screen, GetSystemMetrics failed: " +
                    win32::getErrorMsg());

            if(((minW != 0 && minH != 0) && (width < minW || height < minH)) ||
                ((maxW != 0 && maxH != 0) && (width > maxW || height > maxH)))
                return;

            if(!SetRect(&rect, 0, 0, width, height))
                throw wm::Exception("Can't set window rectangle for resizing");
        } else
        {
            if(!CopyRect(&rect, &impl->windowedRect))
                throw wm::Exception("Can't switch to windowed mode, SetRect failed: " + win32::getErrorMsg());

            if(!AdjustWindowRectEx(&rect, style, false, exstyle))
                throw wm::Exception("Can't switch to windowed mode, AdjustWindowRectEx failed: " + win32::getErrorMsg());

            unsigned int width = rect.right - rect.left;
            unsigned int height = rect.bottom - rect.top;

            rect.left = impl->windowedPosX;
            rect.top = impl->windowedPosY;

            if(minW != 0 && minH != 0)
            {
                if(width < minW) width = minW;
                if(height < minH) height = minH;
            }

            if(maxW != 0 && maxH != 0)
            {
                if(width > maxW) width = maxW;
                if(height > maxH) height = maxH;
            }

            rect.right = rect.left + width;
            rect.bottom = rect.top + height;
        }

        hide();

        if(!SetWindowLong(impl->hwnd, GWL_EXSTYLE, exstyle)
            || !SetWindowLong(impl->hwnd, GWL_STYLE, style))
            throw wm::Exception("Can't set full screen, SetWindowLong failed: " + win32::getErrorMsg());

        if(!SetWindowPos(
            impl->hwnd,
            HWND_TOPMOST,
            rect.left, rect.top,
            rect.right - rect.left, rect.bottom - rect.top,
            SWP_FRAMECHANGED | SWP_SHOWWINDOW))
            throw wm::Exception("Can't set full screen, SetWindowPos failed: " + win32::getErrorMsg());

        impl->fullscreen = full;
    }

    void Window::warpMouse(unsigned int x, unsigned int y)
    {
        RECT rect;
        if(!GetClientRect(impl->hwnd, &rect))
            throw wm::Exception("Can't warp mouse, GetClientRect failed: " + win32::getErrorMsg());

        if(LONG(x) >= rect.right || LONG(y) >= rect.bottom) return;

        POINT point = { 0, 0 };
        if(!ClientToScreen(impl->hwnd, &point))
            throw wm::Exception("Can't warp mouse, ClientToScreen failed: " + win32::getErrorMsg());

        if(!SetCursorPos(point.x + x, point.y + y))
            throw wm::Exception("Can't set cursor position, SetCursorPos failed: " + win32::getErrorMsg());
    }

    void Window::showCursor(bool show)
    {
        // TODO: synchronize this!
        // the critical section is between EventReader mouse stuff
        // and Window::impl_t::cursorVisible in Window::showCursor()

        if(impl->cursorVisible != show)
        {
            if(impl->eventreader.mouseinside)
            {
                ShowCursor(show);
            }

            impl->cursorVisible = show;
        }
    }

    void Window::setMinMaxSize(unsigned int minW, unsigned int minH, unsigned int maxW, unsigned int maxH)
    {
        // TODO: synchronize this!
        // The EventReader reads these on WM_GETMINMAXINFO
        // make sure the read is atomic
        impl->minW = minW; impl->minH = minH;
        impl->maxW = maxW; impl->maxH = maxH;

        int style = 0;
        bool resizable = !(minW && minH && minW && maxH &&
            (minW == maxW) && (minH == maxH));

        if(resizable != impl->resizable)
        {
            LONG style = getStyle(impl->fullscreen, resizable);

            hide();
            if(!SetWindowLong(impl->hwnd, GWL_STYLE, style))
                throw wm::Exception("Can't set window resizable style: " + win32::getErrorMsg());
            show();
        }

        impl->resizable = resizable;
    }

    void Window::repaint(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
    {
        RECT rect, *ptr = 0;

        if(width != 0 && height != 0)
        {
            if(!SetRect(&rect, x, y, width, height))
                throw wm::Exception("Can't repaint Window, SetRect failed: " + win32::getErrorMsg());
            ptr = &rect;
        }

        if(!RedrawWindow(impl->hwnd, ptr, 0, RDW_INVALIDATE))
            throw wm::Exception("Can't repaint Window, RedrawWindow failed: " + win32::getErrorMsg());
    }
}
