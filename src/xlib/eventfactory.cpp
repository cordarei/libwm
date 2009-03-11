#include <iostream>
#include <map>

#include <X11/Xlib.h>

#include <wm/event.hpp>
#include <wm/exception.hpp>

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
			event.xbutton.button,
			event.type == ButtonPress);
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
	
}

#include <boost/cstdint.hpp>

namespace
{
	boost::uint32_t decode_utf8(const unsigned char* data, int len)
	{
		if(len == 0) return 0;
		else if(len == 1
			&& (data[0] & 0x80) == 0
				) return data[0];
		else if(len == 2
			&& (data[0] & 0xe0) == 0xc0
			&& (data[1] & 0xc0) == 0x80
				) return ((data[0] & 0x1f) << 6) | (data[1] & 0x3f);
		else if(len == 3
			&& (data[0] & 0xf0) == 0xe0
			&& (data[1] & 0xc0) == 0x80
			&& (data[2] & 0xc0) == 0x80
				) return ((data[0] & 0x0f) << 12) | ((data[1] & 0x3f) << 6) | (data[2] & 0x3f); 
		else if(len == 4
			&& (data[0] & 0xf7) == 0xf0
			&& (data[1] & 0x3f) == 0x80
			&& (data[2] & 0x3f) == 0x80
			&& (data[3] & 0x3f) == 0x80
				) return ((data[0] & 0x7) << 18) | ((data[1] & 0x3f) << 12) | ((data[2] & 0x3f) << 6) | (data[3] & 0x3f);
		throw wm::Exception("Invalid UTF-8");
	}
}

#include <wm/display.hpp>
#include <wm/window.hpp>
#include "impl/window_impl.hpp"
#include "impl/display_impl.hpp"
#include "impl/eventfactory.hpp"

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
				if(event.type == KeyPress && !filter)
				{
					const size_t buffer_size = 6;
					char buffer[buffer_size] = { 0, 0, 0, 0, 0, 0 };
					KeySym keysym;
					Status status;
					
					
					// XmbLookupString output encoding depends on current locale
					// XwcLookupString also uses locale and nonportable wchar_t
					// Xutf8LookupString always returns UTF-8 and we always want unicode, so use it
					int len = Xutf8LookupString(
						window.impl->xic,
						const_cast<XKeyEvent*>(&event.xkey), // Xlib is not const correct
						buffer,
						buffer_size-1,
						&keysym,
						&status
						);
					buffer[len] = 0;	// add null terminator
					
					switch(status)
					{
					case XBufferOverflow:
						throw wm::Exception("Xutf8LookupString status XBufferOverflow"); // 6 bytes should be enough, this should never happen
					case XLookupNone:
						return 0;
					case XLookupKeySym:
					case XLookupBoth:	
						
						{
							KeyEvent *key = new KeyEvent(window, true);		// TODO: handle keysym
							if(status == XLookupKeySym) return key;
							// Propagate KeyEvent before CharacterEvent
							window.impl->eventq.push(key); 	// eventq handles the destruction of the pushed event
						}
					case XLookupChars:
					
						return new wm::CharacterEvent(
							window,
							decode_utf8(
								reinterpret_cast<const unsigned char*>(buffer),
								len
							));
					}
				}
			
				// TODO: lookup and handle keysym
				
				return new wm::KeyEvent(
					window,
					event.type == KeyPress);
			}


	};

	namespace xlib
	{
		long event_mask = 
			StructureNotifyMask |
			ExposureMask |
//			PointerMotionMask |
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

