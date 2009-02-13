#ifndef WM_EXCEPTION_HPP
#define WM_EXCEPTION_HPP

#include <stdexcept>

namespace wm
{
	struct Exception : public std::runtime_error
	{
		Exception(const char *msg)
			: std::runtime_error(msg)
		{
		}
	};
}

#endif

