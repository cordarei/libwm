#ifndef DISPLAY_IMPL_HPP
#define DISPLAY_IMPL_HPP

#include <string>

namespace wm
{
	struct Display::impl_t
	{
		explicit impl_t(const std::string &classname)
			: classname(classname)
		{
		}

		std::string classname;
		HINSTANCE hInstance;
	};
}

#endif
