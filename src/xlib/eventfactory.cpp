#include <map>

#include <X11/Xlib.h>

#include <wm/event.hpp>
#include <wm/events.hpp>

#include <xlib/impl/keymap.hpp>

namespace 
{
	const wm::Event* makeExpose(
		wm::Window& window,
		const XEvent &event,
		bool)
	{
		return new wm::ExposeEvent(
			window,
			event.xexpose.x,
			event.xexpose.y,
			event.xexpose.width,
			event.xexpose.height);
	}


	const wm::Event* makeButton(
		wm::Window& window,
		const XEvent &event,
		bool)
	{
		return new wm::ButtonEvent(
			window,
			event.xbutton.x,
			event.xbutton.y,
			wm::xlib::mapButton(event.xbutton.button),
			event.type == ButtonPress,
			wm::xlib::mapButtons(event.xmotion.state),
			wm::xlib::mapKeyMod(event.xmotion.state));
	}
	
	const wm::Event* makeFocus(
		wm::Window& window,
		const XEvent &event,
		bool)
	{
		return new wm::FocusEvent(
			window,
			event.type == FocusIn);
	}
	
	const wm::Event* makeMouseOver(
		wm::Window& window,
		const XEvent &event,
		bool)
	{
		return new wm::MouseOverEvent(
			window,
			event.xcrossing.x,
			event.xcrossing.y,
			event.type == EnterNotify);
	}
		
	const wm::Event* makeShow(
		wm::Window& window,
		const XEvent &event,
		bool)
	{
		return new wm::ShowEvent(
			window,
			event.type == MapNotify
			);
	}

	const wm::Event* makeMotion(
		wm::Window& window,
		const XEvent &event,
		bool)
	{
		return new wm::MotionEvent(
			window,
			event.xmotion.x,
			event.xmotion.y,
			wm::xlib::mapButtons(event.xmotion.state),
			wm::xlib::mapKeyMod(event.xmotion.state));
	}
}

namespace wm
{
	namespace xlib
	{
		const Event* fromXEvent(
			wm::Window& window,
			const XEvent &event,
			bool filter
			)
		{
			typedef const Event* (DispatchFunc)(
				wm::Window& window,
				const XEvent &,
				bool);

			static const struct Registry
			{
				Registry()
				{
					map[Expose] = makeExpose;
					map[ButtonPress] = makeButton;
					map[ButtonRelease] = makeButton;
					map[FocusIn] = makeFocus;
					map[FocusOut] = makeFocus;
					map[EnterNotify] = makeMouseOver;
					map[LeaveNotify] = makeMouseOver;
					map[MapNotify] = makeShow;
					map[UnmapNotify] = makeShow;
					map[MotionNotify] = makeMotion;					
				}
			
				typedef std::map<int, DispatchFunc*> map_t;
				map_t map;
			} registry;

			Registry::map_t::const_iterator iter =
				registry.map.find(event.type);
			if(iter != registry.map.end())
			{
				DispatchFunc *func = iter->second;
				return func(window, event, filter);
			} 

			return 0;
		}
	}
}

