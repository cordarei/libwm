#ifndef WM_UTIL_UTF8_HPP
#define WM_UTIL_UTF8_HPP

#include <string>

#include <boost/cstdint.hpp>

#include <wm/exception.hpp>

namespace wm
{
	namespace util
	{
		/// Convert UTF-8 character to unicode code point
		/**
			@param data character encoded in UTF-8
			@param len number of bytes in UTF-8 data
			@return Unicode code point or 0 if len == 0
			@throws wm::Exception if data is NULL or contains invalid UTF-8
		*/
		inline boost::uint32_t decode_utf8(const unsigned char* data, int len)
		{
			if(!data)
				throw wm::Exception("wm::util::decode_utf8: NULL data");
		
			if(len == 0) return 0;
			else if(len == 1
				&& (data[0] & 0x80) == 0
					) return data[0];
			else if(len == 2
				&& (data[0] & 0xe0) == 0xc0
				&& (data[1] & 0xc0) == 0x80
					) return ((data[0] & 0x1f) << 6) | (data[1] & 0x3f);
			else if(len == 3
				&& (data[0] & 0xf0) == 0xe0
				&& (data[1] & 0xc0) == 0x80
				&& (data[2] & 0xc0) == 0x80
					) return ((data[0] & 0x0f) << 12) | ((data[1] & 0x3f) << 6) | (data[2] & 0x3f); 
			else if(len == 4
				&& (data[0] & 0xf7) == 0xf0
				&& (data[1] & 0x3f) == 0x80
				&& (data[2] & 0x3f) == 0x80
				&& (data[3] & 0x3f) == 0x80
					) return ((data[0] & 0x7) << 18) | ((data[1] & 0x3f) << 12) | ((data[2] & 0x3f) << 6) | (data[3] & 0x3f);
			throw wm::Exception("Invalid UTF-8");
		}
		
		/// Convert a Unicode character to UTF-8
		/**
			@param codepoint the Unicode code point to encode
			@param out output buffer, must have at least 4 bytes allocated
			@return the number of bytes written
			@throws wm::Exception if invalid Unicode character
		*/	
		inline int encode_utf8(boost::uint32_t codepoint, unsigned char *out)
		{
			if(!out)
				throw wm::Exception("wm::util::encode_utf8: NULL output buffer");
			
			typedef unsigned char uchar;
	
			if(!(codepoint & ~0x7F))
			{
				out[0] = uchar(codepoint & 0x7F);
				return 1;
			} else if(!(codepoint & ~0x7FF))
			{
				out[0] = uchar(0xc0 | (codepoint & 0x7C0) >> 6);
				out[1] = uchar(0x80 | (codepoint & 0x3F));
				return 2;
			} else if(!(codepoint & ~0xFFFF))
			{
				out[0] = uchar(0xe0 | (codepoint & 0xF000) >> 12);
				out[1] = uchar(0x80 | (codepoint & 0xFC) >> 6);
				out[2] = uchar(0x80 | (codepoint & 0x3F));
				return 3;
			} else if(!(codepoint & ~0x3FFFF))
			{
				out[0] = uchar(0xf0 | (codepoint & 0x1c0000) >> 18);
				out[1] = uchar(0x80 | (codepoint & 0x3f00) >> 12);
				out[2] = uchar(0x80 | (codepoint & 0xFC) >> 6);
				out[3] = uchar(0x80 | (codepoint & 0x3F));
				return 4;
			}
		
			throw std::runtime_error("Invalid Unicode codepoint");
		}
	
		/// Convert a Unicode character to UTF-8 string
		/**
			@param codepoint the Unicode code point to encode
			@return Unicode character encoded in UTF-8 string
			@throws wm::Exception if invalid Unicode character
		*/
		inline std::string encode_utf8(boost::uint32_t codepoint)
		{
			unsigned char buf[4];
			int len = encode_utf8(codepoint, buf);
			return std::string(
				reinterpret_cast<char*>(buf+0),
				reinterpret_cast<char*>(buf+len)
				);
		}
	
	}
}

#endif

