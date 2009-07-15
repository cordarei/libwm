#include <map>

#include <boost/cstdint.hpp>

#include <wm/exception.hpp>
#include <wm/event.hpp>
#include <wm/events.hpp>
#include <wm/mouse.hpp>
#include <wm/keyboard.hpp>
#include "impl/eventfactory.hpp"
#include "impl/keymap.hpp"

#include <windows.h>

namespace
{
#undef MOD_CONTROL		// hairy win32 #defines cause compilation errors
#undef MOD_SHIFT
	wm::keyboard::KeyMod mapKeyMod(WPARAM wparam)
	{	
		return 0
			| ((wparam & MK_CONTROL) ? wm::keyboard::MOD_CONTROL : 0)
			| ((wparam & MK_SHIFT) ? wm::keyboard::MOD_SHIFT : 0)
			;
	}

	wm::mouse::ButtonMask mapButtons(WPARAM wparam)
	{
		return 0 
			| ((wparam & MK_LBUTTON) ? wm::mouse::MASK_LEFT : 0)
			| ((wparam & MK_MBUTTON) ? wm::mouse::MASK_MIDDLE : 0)
			| ((wparam & MK_RBUTTON) ? wm::mouse::MASK_RIGHT : 0)
			;
	}


	const wm::Event *makeButton(
		wm::Window& window,
		UINT message,
		WPARAM wparam,
		LPARAM lparam)
	{
		unsigned int x = LOWORD(lparam),
					 y = HIWORD(lparam);

		wm::mouse::Button button = wm::mouse::UNKNOWN;
		bool state = false;
		switch(message)
		{
		case WM_LBUTTONDOWN:
			button = wm::mouse::BUTTON_LEFT; state = true;
			break;
		case WM_LBUTTONUP:
			button = wm::mouse::BUTTON_LEFT; state = false;
			break;

		case WM_MBUTTONDOWN:
			button = wm::mouse::BUTTON_MIDDLE; state = true;
			break;
		case WM_MBUTTONUP:
			button = wm::mouse::BUTTON_MIDDLE; state = false;
			break;

		case WM_RBUTTONDOWN:
			button = wm::mouse::BUTTON_RIGHT; state = true;
			break;
		case WM_RBUTTONUP:
			button = wm::mouse::BUTTON_RIGHT; state = false;
			break;

		case WM_XBUTTONDOWN:
			button = wm::mouse::Button(wm::mouse::BUTTON_X + GET_XBUTTON_WPARAM(wparam) - 1);
			state = true;
			break;
		case WM_XBUTTONUP:
			button = wm::mouse::Button(wm::mouse::BUTTON_X + GET_XBUTTON_WPARAM(wparam) - 1);
			state = false;
			break;

		case WM_MOUSEWHEEL:
			button = GET_WHEEL_DELTA_WPARAM(wparam) < 0 
				? wm::mouse::WHEEL_DOWN : wm::mouse::WHEEL_UP;
			state = true;
			break;

		case WM_MOUSEHWHEEL:
			button = GET_WHEEL_DELTA_WPARAM(wparam) < 0
				? wm::mouse::SCROLL_LEFT : wm::mouse::SCROLL_RIGHT;
			state = true;
			break;

		}

		wm::keyboard::KeyMod keymod = mapKeyMod(wparam)
			| (wm::win32::getKeyModState() &
				~(wm::keyboard::MOD_SHIFT | wm::keyboard::MOD_CONTROL));

		return new wm::ButtonEvent(
			window,
			x, y,
			button,
			state,
			mapButtons(wparam),
			keymod);
	}

	const wm::Event *makeFocus(
		wm::Window& window,
		UINT message,
		WPARAM wparam,
		LPARAM lparam)
	{
		return new wm::FocusEvent(window, message == WM_SETFOCUS ? true : false);
	}

	const wm::Event *makeClose(
		wm::Window& window,
		UINT message,
		WPARAM wparam,
		LPARAM lparam)
	{
		return new wm::CloseEvent(window);
	}

	const wm::Event *makeChar(
		wm::Window& window,
		UINT message,
		WPARAM wparam,
		LPARAM lparam)
	{
		boost::uint16_t lo = wparam & 0xFFFF;
		boost::uint16_t hi = (wparam & 0xFFFF0000) >> 16;
		boost::uint32_t codepoint = 0;

		// Convert from UTF-16 to Unicode codepoint
		if(lo < 0xD8000 || lo > 0xDFFF) codepoint = lo;
		else if(lo < 0xD800 || lo > 0xDBFF) throw wm::Exception("Win32 WM_CHAR message contains invalid UTF-16");
		else if(hi < 0xDC00 || hi > 0xDFFF) throw wm::Exception("Win32 WM_CHAR message contains invalid UTF-16");
		else codepoint = ((lo & 0x3FF) << 10) | (hi & 0x3FF) + 0x10000;

		return new wm::CharacterEvent(window, codepoint);
	}

	const wm::Event *makeMotion(
		wm::Window& window,
		UINT message,
		WPARAM wparam,
		LPARAM lparam)
	{
		unsigned int x = LOWORD(lparam);
		unsigned int y = HIWORD(lparam);

		wm::keyboard::KeyMod keymod = mapKeyMod(wparam)
			| (wm::win32::getKeyModState() &
				~(wm::keyboard::MOD_SHIFT | wm::keyboard::MOD_CONTROL));

		return new wm::MotionEvent(
			window,
			x, y,
			mapButtons(wparam),
			keymod
			);
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
					map[WM_CHAR] = &makeChar;

					map[WM_LBUTTONDOWN]= &makeButton;
					map[WM_LBUTTONUP]= &makeButton;
					map[WM_MBUTTONDOWN]= &makeButton;
					map[WM_MBUTTONUP]= &makeButton;
					map[WM_RBUTTONDOWN]= &makeButton;
					map[WM_RBUTTONUP]= &makeButton;
					map[WM_XBUTTONDOWN]= &makeButton;
					map[WM_XBUTTONUP]= &makeButton;
					map[WM_MOUSEWHEEL]= &makeButton;
					map[WM_MOUSEHWHEEL]= &makeButton;
					
					map[WM_MOUSEMOVE] = &makeMotion;

					map[WM_SETFOCUS] = &makeFocus;
					map[WM_KILLFOCUS] = &makeFocus;

					map[WM_CLOSE] = &makeClose;
				}

				typedef std::map<UINT, FactoryFunc*> map_t;
				map_t map;
			} registry;

			typedef temp_t::map_t::const_iterator iterator;
			iterator i = registry.map.find(message);
			
			if(i == registry.map.end())
				return 0;

			FactoryFunc* factory = i->second;
			if(factory) return factory(window, message, wparam, lparam);

			return 0;
		}
	}
}
