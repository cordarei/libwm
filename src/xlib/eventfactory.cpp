#include <iostream>
#include <map>

#include <X11/Xlib.h>

#include <wm/event.hpp>
#include <wm/events.hpp>
#include <wm/exception.hpp>
#include "impl/keymap.hpp"

namespace 
{
	const char *event_names[] = {
	   "",
	   "",
	   "KeyPress",
	   "KeyRelease",
	   "ButtonPress",
	   "ButtonRelease",
	   "MotionNotify",
	   "EnterNotify",
	   "LeaveNotify",
	   "FocusIn",
	   "FocusOut",
	   "KeymapNotify",
	   "Expose",
	   "GraphicsExpose",
	   "NoExpose",
	   "VisibilityNotify",
	   "CreateNotify",
	   "DestroyNotify",
	   "UnmapNotify",
	   "MapNotify",
	   "MapRequest",
	   "ReparentNotify",
	   "ConfigureNotify",
	   "ConfigureRequest",
	   "GravityNotify",
	   "ResizeRequest",
	   "CirculateNotify",
	   "CirculateRequest",
	   "PropertyNotify",
	   "SelectionClear",
	   "SelectionRequest",
	   "SelectionNotify",
	   "ColormapNotify",
	   "ClientMessage",
	   "MappingNotify"
		};

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

#include <wm/display.hpp>
#include <wm/window.hpp>
#include "impl/window_impl.hpp"
#include "impl/display_impl.hpp"
#include "impl/eventfactory.hpp"

#include <wm/util/utf8.hpp>

namespace wm
{
	class EventReader
	{
		public:
			static const Event* makeClient(
				wm::Window &window,
				const XEvent &event,
				bool)
			{
				if(event.xclient.data.l[0] ==
					window.display().impl->wm_delete_window)
				{
					return new wm::CloseEvent(window);
				}
				
				return 0;
			}
			
			static const wm::Event* makeResize(
				wm::Window& window,
				const XEvent &event,
				bool)
			{
				if(event.xconfigure.width == window.impl->width &&
					event.xconfigure.height == window.impl->height)
					return 0;

				window.impl->width = event.xconfigure.width;	
				window.impl->height = event.xconfigure.height;	

				return new wm::ResizeEvent(
					window,
					event.xconfigure.width,
					event.xconfigure.height
					);
			}
			
			static const wm::Event* makeKey(
				wm::Window& window,
				const XEvent &event,
				bool filter)
			{
				const int keycode_index = 0; // Ignore modmask for keysym
				KeySym keysym = XKeycodeToKeysym(
					window.display().impl->display,
					event.xkey.keycode,
					keycode_index);
			
				if(event.type == KeyPress && !filter)
				{
					const size_t buffer_size = 6;
					char buffer[buffer_size] = { 0, 0, 0, 0, 0, 0 };
					Status status;
					
					// XmbLookupString output encoding depends on current locale
					// XwcLookupString also uses locale and nonportable wchar_t
					// Xutf8LookupString always returns UTF-8 and we always want unicode, so use it
					int len = Xutf8LookupString(
						window.impl->xic,
						const_cast<XKeyEvent*>(&event.xkey), // Xlib is not const correct
						buffer,
						buffer_size-1,
						0,  // pointer to keysym, NULL for no keysym lookup
						&status
						);
					buffer[len] = 0;	// add null terminator
					
					if(status == XBufferOverflow) // 6 bytes should be enough, this should never happen
						throw wm::Exception("Xutf8LookupString status XBufferOverflow");

					if(status == XLookupBoth || status == XLookupChars)
					{
						// X input method (XIM) tells us that characters have been written
						// First, propagate KeyEvent to event queue
						KeyEvent *key = new KeyEvent(
							window,
							xlib::mapXKeySym(keysym),
							xlib::mapKeyMod(event.xkey.state),
							true);
						window.impl->eventq.push(key); 	// eventq handles the destruction of the pushed 
						
						// Then return CharacterEvent
						return new wm::CharacterEvent(
							window,
							wm::util::decode_utf8(
								reinterpret_cast<const unsigned char*>(buffer),
								len
							));
					}
				}
			
				return new wm::KeyEvent(
					window,
					xlib::mapXKeySym(keysym),
					xlib::mapKeyMod(event.xkey.state),
					event.type == KeyPress);
			}
	};

	namespace xlib
	{
		long event_mask = 
			StructureNotifyMask |
			PointerMotionMask |
			ButtonPressMask |
			ButtonReleaseMask |
			KeyPressMask |
			KeyReleaseMask |
			EnterWindowMask |
			LeaveWindowMask |
			FocusChangeMask
			;

	
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
					
					map[KeyPress] = &EventReader::makeKey;
					map[KeyRelease] = &EventReader::makeKey;
					map[ConfigureNotify] = &EventReader::makeResize;
					map[ClientMessage] = &EventReader::makeClient;
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
			} else
			{
				std::cout << "Unhandled event " << event_names[event.type] << std::endl;
				return 0;
			}
		}
	}
}

