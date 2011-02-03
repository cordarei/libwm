#ifndef IMPL_ERROR_HPP
#define IMPL_ERROR_HPP

#include <string>

#include <windows.h>

namespace wm
{
    namespace win32
    {
        std::string getErrorMsg(DWORD err);
        std::string getErrorMsg();
    }
}

#endif
