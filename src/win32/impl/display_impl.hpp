#ifndef DISPLAY_IMPL_HPP
#define DISPLAY_IMPL_HPP

#include <string>

namespace wm
{
	struct Display::impl_t
	{
		std::string classname;
		HINSTANCE hInstance;
	};
}

#endif
