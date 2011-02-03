#ifndef DISPLAY_IMPL_HPP
#define DISPLAY_IMPL_HPP

#include <windows.h>

namespace wm
{
    struct Display::impl_t
    {
        ATOM classatom;
        HINSTANCE hInstance;
    };
}

#endif
