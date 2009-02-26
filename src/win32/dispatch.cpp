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
	void dispatchKey(
		wm::Window& window,
		wm::common::Dispatcher& dispatcher,
		UINT message,
		WPARAM wparam,
		LPARAM lparam)
	{
		wm::KeyEvent key(window, message == WM_KEYDOWN);
		dispatcher.dispatch(key);
	}

	void dispatchPaint(
		wm::Window& window,
		wm::common::Dispatcher& dispatcher,
		UINT message,
		WPARAM wparam,
		LPARAM lparam)
	{
		// TODO: query window width or remove coordinates from ExposeEvent altogether
		wm::ExposeEvent expose(window, 0, 0, 0, 0);
		dispatcher.dispatch(expose);
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
					map[WM_PAINT] = &dispatchPaint;
				}

				typedef std::map<UINT, DispatcherFunc*> map_t;
				map_t map;
			} registry;

			typedef temp_t::map_t::const_iterator iterator;
			iterator i = registry.map.find(message);
			if(i == registry.map.end()) return 0;
			else return i->second;
		}

		bool dispatchEvent(
			Window& window,
			common::Dispatcher& dispatcher,
			UINT message,
			WPARAM wparam,
			LPARAM lparam)
		{
			DispatcherFunc *func = getDispatcher(message);
			if(func) func(window, dispatcher, message, wparam, lparam);
			return func ? true : false;
		}
	}
}
