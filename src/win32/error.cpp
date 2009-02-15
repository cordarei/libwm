#include <boost/lexical_cast.hpp>

#include <windows.h>

#include "impl/error.hpp"

namespace wm
{
	namespace win32
	{
		std::string getErrorMsg()
		{
			DWORD err = GetLastError();

			LPVOID msgBuf = 0;
			try
			{
				if(!FormatMessage(
					FORMAT_MESSAGE_ALLOCATE_BUFFER | 
					FORMAT_MESSAGE_FROM_SYSTEM |
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					err,
					0,
					(LPTSTR) &msgBuf,
					0, NULL ))
				{
					return "FormatMessage failed with error code: " +
						boost::lexical_cast<std::string>(GetLastError());
				}

				std::string msg(reinterpret_cast<const char *>(msgBuf));
				LocalFree(msgBuf);
				return msg;
			} catch(...)
			{
				LocalFree(msgBuf);
				return "Can't get Win32 error message";
			}
		}
	}
}
