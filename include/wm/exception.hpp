#ifndef WM_EXCEPTION_HPP
#define WM_EXCEPTION_HPP

#include <stdexcept>
#include <string>

namespace wm
{
	/// An exception
	struct Exception : public std::runtime_error
	{
		/// Create a new Exception
		/**
			@param msg the Exception message
		*/
		explicit Exception(const std::string& msg)
			: std::runtime_error(msg)
		{
		}
	};
}

#endif

