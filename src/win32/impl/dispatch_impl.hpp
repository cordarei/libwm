#ifndef DISPATCHER_IMPL_HPP
#define DISPATCHER_IMPL_HPP

#include <windows.h>

namespace wm
{
	class Window;

	namespace common
	{
		class Dispatcher;
	}

	namespace win32
	{
		typedef void (DispatcherFunc)(
			wm::Window& window,
			wm::common::Dispatcher& dispatcher,
			UINT message,
			WPARAM wparam,
			LPARAM lparam);

		DispatcherFunc *getDispatcher(UINT message);
		bool dispatchEvent(
			Window&,
			common::Dispatcher&,
			UINT message,
			WPARAM wparam,
			LPARAM lparam);
	}
}

#endif
