#include <map>

#include <X11/Xlib.h>

#include <wm/display.hpp>
#include <wm/window.hpp>
#include <wm/event.hpp>
#include <wm/events.hpp>

#include <common/eventqueue.hpp>
#include <xlib/impl/eventreader.hpp>
#include <xlib/impl/display_impl.hpp>
#include <xlib/impl/window_impl.hpp>
#include <xlib/impl/eventfactory.hpp>
#include <xlib/impl/keymap.hpp>

#include <wm/util/utf8.hpp>

namespace wm
{
	const long EventReader::event_mask = 0
			| StructureNotifyMask
			| PointerMotionMask
			| ButtonPressMask
			| ButtonReleaseMask
			| KeyPressMask
			| KeyReleaseMask
			| EnterWindowMask
			| LeaveWindowMask
			| FocusChangeMask
			| ExposureMask
			;
			
	void EventReader::handleXEvent(wm::Window& window, const XEvent& event)
	{
		EventReader &reader = window.impl->eventreader;
		bool filter = XFilterEvent(const_cast<XEvent*>(&event), None);
		
		if(!reader.handleSpecial(window, event, filter))
		{
			const Event *ptr = xlib::fromXEvent(window, event, filter);
			if(ptr) window.impl->eventq.push(ptr); // eventq takes ownership of event object in ptr
		}
	}
	
	bool EventReader::handleSpecial(wm::Window& window, const XEvent &event, bool filter)
	{
		typedef void (EventReader::*HandlerFunc)(wm::Window&, const XEvent&, bool);		
		
		static const struct Registry
		{
			Registry()
			{
				map[ClientMessage] = &EventReader::handleClientMessage;
				map[ConfigureNotify] = &EventReader::handleConfigureNotify;
				map[KeyPress] = &EventReader::handleKeyEvent;
				map[KeyRelease] = &EventReader::handleKeyEvent;
			}
					
			typedef std::map<int, HandlerFunc> map_t;
			map_t map;
		} registry;
		
		Registry::map_t::const_iterator iter = registry.map.find(event.type);
		if(iter == registry.map.end()) return false;
		
		HandlerFunc handler = iter->second;
		(this->*handler)(window, event, filter);
		
		return true;
	}

	void EventReader::handleClientMessage(wm::Window& window, const XEvent &event, bool filter)
	{
		xlib::EWMH &ewmh = window.display().impl->ewmh;
		
		if(event.xclient.data.l[0] ==
			ewmh.wm_delete_window)
		{
			window.impl->eventq.push(new wm::CloseEvent(window));
		}
	}

	void EventReader::handleKeyEvent(wm::Window& window, const XEvent &event, bool filter)
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
				window.impl->eventq.push(
					new KeyEvent(
						window,
						xlib::mapXKeySym(keysym),
						xlib::mapKeyMod(event.xkey.state),
						true));
				
				// Then propagate CharacterEvent
				window.impl->eventq.push(
					new wm::CharacterEvent(
						window,
						wm::util::decode_utf8(
							reinterpret_cast<const unsigned char*>(buffer),
							len
						)));

				return;
			}
		}
		
		window.impl->eventq.push(
			new wm::KeyEvent(
				window,
				xlib::mapXKeySym(keysym),
				xlib::mapKeyMod(event.xkey.state),
				event.type == KeyPress));
	}
	
	void EventReader::handleConfigureNotify(wm::Window& window, const XEvent &event, bool filter)
	{
		if(event.xconfigure.width == width &&
			event.xconfigure.height == height)
			return;

		width = event.xconfigure.width;	
		height = event.xconfigure.height;	

		window.impl->eventq.push(
			new wm::ResizeEvent(
				window,
				event.xconfigure.width,
				event.xconfigure.height
				));
	}
}


