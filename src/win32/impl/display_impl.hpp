#ifndef DISPLAY_IMPL_HPP
#define DISPLAY_IMPL_HPP

#include <vector>

#include <windows.h>

namespace wm
{
	struct Display::impl_t
	{
		std::vector<WCHAR> classname;
		HINSTANCE hInstance;
	};
}

#endif
