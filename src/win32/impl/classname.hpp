#ifndef WIN32_IMPL_CLASSNAME_HPP
#define WIN32_IMPL_CLASSNAME_HPP

#include <vector>

#include <windows.h>

namespace wm
{
	namespace win32
	{
		inline char getHexChar(unsigned int idx)
		{
			if(idx >= 16) return '!';
			return (idx < 10)
				? '0' + idx
				: 'a' + (idx - 10);
		}

		// Simple hex string representation of a pointer
		inline std::vector<WCHAR> genClassNameStr(const void* ptr)
		{
			size_t size = sizeof(const void*);
			const unsigned char *begin =
				reinterpret_cast<const unsigned char *>(&ptr);

			std::vector<WCHAR> str;
			str.reserve(2 * size + 1);
			for(const unsigned char *ptr = begin;
				ptr != begin + size;
				++ptr)
			{
				str.push_back(getHexChar(*ptr & 0x0f)); // put least significant digit first for added obscurity
				str.push_back(getHexChar(*ptr & 0xf0 >> 4));
			}
			str.push_back(0);

			return str;
		}
	}
}

#endif
