#ifndef WGL_IMPL_EXTENSIONS_HPP
#define WGL_IMPL_EXTENSIONS_HPP

#include <string>
#include <set>

#include <windows.h>

namespace wm
{
	namespace wgl
	{
		struct Extensions
		{
			void init(HINSTANCE hinstance);
			bool supported(const std::string &name) const;

			std::set<std::string> extensions;

			const char* (WINAPI * wglGetExtensionsStringARB) (HDC hdc);
		};
	}
}

#endif
