#ifndef EVENTFACTORY_IMPL_HPP
#define EVENTFACTORY_IMPL_HPP

#include <windows.h>

namespace wm
{
	class Window;
	class Event;

	namespace win32
	{
		const Event* fromWin32Event(
			Window& window,
			UINT message,
			WPARAM wparam,
			LPARAM lparam);
	}
}

#endif
