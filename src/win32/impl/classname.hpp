#ifndef WIN32_IMPL_CLASSNAME_HPP
#define WIN32_IMPL_CLASSNAME_HPP

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
		inline void genClassNameStr(const void *ptr, WCHAR *out)
		{
			size_t size = sizeof(const void*);
			const unsigned char *begin =
				reinterpret_cast<const unsigned char *>(&ptr);

			for(const unsigned char *ptr = begin;
				ptr != begin + size;
				++ptr)
			{
				*(out++) = getHexChar(*ptr & 0x0f); // put least significant digit first for added obscurity
				*(out++) = getHexChar((*ptr & 0xf0) >> 4);
			}
			*(out++) = 0;
		}
	}
}

#endif
