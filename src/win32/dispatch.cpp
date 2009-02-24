#include <map>

#include <wm/event.hpp>

#include <common/dispatcher.hpp>

#include "impl/dispatch_impl.hpp"

namespace wm
{
	class Window;
}

namespace
{
	void dispatchKey(wm::Window& window, wm::common::Dispatcher& dispatcher, const MSG& msg)
	{
		wm::KeyEvent key(window, msg.message == WM_KEYDOWN);
		dispatcher.dispatch(key);
	}
}

namespace wm
{
	namespace win32
	{
		DispatcherFunc *getDispatcher(UINT message)
		{
			static struct temp_t
			{
				temp_t()
				{
					map[WM_KEYDOWN] = &dispatchKey;
				}

				typedef std::map<UINT, DispatcherFunc*> map_t;
				map_t map;
			} registry;

			typedef temp_t::map_t::const_iterator iterator;
			iterator i = registry.map.find(message);
			if(i == registry.map.end()) return 0;
			else return i->second;
		}

		void dispatchEvent(Window& window, common::Dispatcher& dispatcher, const MSG& msg)
		{
			DispatcherFunc *func = getDispatcher(msg.message);
			if(func) func(window, dispatcher, msg);
		}
	}
}
