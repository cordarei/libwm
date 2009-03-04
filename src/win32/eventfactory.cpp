#include <map>

#include <windows.h>

#include <wm/event.hpp>
#include "impl/eventfactory.hpp"

namespace
{
	const wm::Event *makePaint(
		wm::Window& window,
		UINT message,
		WPARAM wparam,
		LPARAM lparam)
	{
		return new wm::ExposeEvent(window, 0, 0, 0, 0);
	}

	const wm::Event *makeKey(
		wm::Window& window,
		UINT message,
		WPARAM wparam,
		LPARAM lparam)
	{
		return new wm::KeyEvent(window, message==WM_KEYDOWN);
	}
}

namespace wm
{
	namespace win32
	{
		const Event* fromWin32Event(
			Window& window,
			UINT message,
			WPARAM wparam,
			LPARAM lparam)
		{
			typedef const Event*
				(FactoryFunc) (
					Window& window,
					UINT message,
					WPARAM wparam,
					LPARAM lparam);

			static struct temp_t
			{
				temp_t()
				{
					map[WM_KEYDOWN] = &makeKey;
					map[WM_PAINT] = &makePaint;
				}

				typedef std::map<UINT, FactoryFunc*> map_t;
				map_t map;
			} registry;

			typedef temp_t::map_t::const_iterator iterator;
			iterator i = registry.map.find(message);
			if(i == registry.map.end()) return 0;

			FactoryFunc* factory = i->second;
			if(factory) return factory(window, message, wparam, lparam);

			return 0;
		}
	}
}
