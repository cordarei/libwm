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
			Window&,
			common::Dispatcher&,
			const MSG&);

		DispatcherFunc *getDispatcher(UINT message);
		void dispatchEvent(Window&, common::Dispatcher&, const MSG&);
	}
}

#endif
