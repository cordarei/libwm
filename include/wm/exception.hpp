#ifndef WM_EXCEPTION_HPP
#define WM_EXCEPTION_HPP

#include <stdexcept>
#include <string>

namespace wm
{
	struct Exception : public std::runtime_error
	{
		Exception(const std::string& msg)
			: std::runtime_error(msg)
		{
		}
	};
}

#endif

