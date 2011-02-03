#include <map>

#include <X11/Xlib.h>

#include <wm/display.hpp>
#include <wm/window.hpp>
#include <wm/event.hpp>

#include <xlib/impl/eventreader.hpp>
#include <xlib/impl/display_impl.hpp>
#include <xlib/impl/window_impl.hpp>
#include <xlib/impl/keymap.hpp>

#include <wm/util/utf8.hpp>

namespace
{
    wm::Event fromXEvent(wm::Window &window, const XEvent& xevent)
    {
        wm::Event event;

        switch(xevent.type)
        {
            case Expose:
                event.expose = { wm::EXPOSE, &window };
                break;
            case ButtonPress:
            case ButtonRelease:
                event.button = {
                    (xevent.type == ButtonPress) ? wm::BUTTON_DOWN : wm::BUTTON_UP,
                    &window,
                    xevent.xbutton.x, xevent.xbutton.y,
                    wm::xlib::mapButton(xevent.xbutton.button),
                    wm::xlib::mapButtons(xevent.xbutton.state),
                    wm::xlib::mapKeyMod(xevent.xbutton.state)
                };
                break;
            case FocusIn:
            case FocusOut:
                event.focus = {
                    (xevent.type == FocusIn) ? wm::FOCUS_GOT : wm::FOCUS_LOST,
                    &window,
                };
                break;
            case EnterNotify:
            case LeaveNotify:
                event.mouseover = {
                    (xevent.type == EnterNotify) ? wm::MOUSE_ENTER : wm::MOUSE_LEAVE,
                    &window,
                    xevent.xcrossing.x,
                    xevent.xcrossing.y
                };
                break;
            case MapNotify:
            case UnmapNotify:
                event.show = {
                    (xevent.type == MapNotify) ? wm::SHOW : wm::HIDE,
                    &window
                };
                break;
            case MotionNotify:
                event.motion = {
                    wm::MOTION,
                    &window,
                    xevent.xmotion.x, xevent.xmotion.y,
                    wm::xlib::mapButtons(xevent.xmotion.state),
                    wm::xlib::mapKeyMod(xevent.xmotion.state)
                };
                break;
            default:
                event.any = { wm::NO_EVENT, nullptr };
                break;
        }

        return event;
    }
}

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

	void EventReader::handleXEvent(wm::Window& window, const XEvent& xevent)
	{
		EventReader &reader = window.impl->eventreader;

        switch(xevent.type)
        {
            case ClientMessage:
                reader.handleClientMessage(window, xevent);
                return;
            case ConfigureNotify:
                reader.handleConfigureNotify(window, xevent);
                return;
            case KeyPress:
            case KeyRelease:
                reader.handleKeyEvent(window, xevent);
                return;
            default:
                window.impl->event_queue->push(fromXEvent(window, xevent));
                break;
        }
	}

    void EventReader::handleClientMessage(wm::Window& window, const XEvent &event)
	{
		xlib::EWMH &ewmh = window.display().impl->ewmh;

		if(static_cast<Atom>(event.xclient.data.l[0]) ==
			ewmh.wm_delete_window)
		{
			// window.impl->eventq.push(new wm::CloseEvent(window));
		}
	}

	void EventReader::handleKeyEvent(wm::Window& window, const XEvent &event)
	{
		bool filter = XFilterEvent(const_cast<XEvent*>(&event), None);

		::Display *xdisplay = window.display().impl->display;
		const int keycode_index = 0; // Ignore modmask for keysym
		KeySym keysym = XKeycodeToKeysym(
			xdisplay,
			event.xkey.keycode,
			keycode_index);

		bool repeat = false;

		if(event.type == KeyPress)
			repeat = (event.xkey.serial == keyrepeat_serial);

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
                /*
				window.impl->eventq.push(
					new KeyEvent(
						window,
						xlib::mapXKeySym(keysym),
						xlib::mapKeyMod(event.xkey.state),
						true,
						repeat));

				// Then propagate CharacterEvent
				window.impl->eventq.push(
					new wm::CharacterEvent(
						window,
						wm::util::decode_utf8(
							reinterpret_cast<const unsigned char*>(buffer),
							len
						)));
                        */
				return;
			}
		}

		if(event.type == KeyRelease)
		{
			struct Predicate
			{
				Predicate(::Window window, unsigned long serial) : window(window), serial(serial) { }

				::Window window;
				unsigned long serial;

				static Bool func(::Display *, XEvent *event, XPointer arg)
				{
					const Predicate &pred = *reinterpret_cast<const Predicate*>(arg);

					return event->type == KeyPress &&
						event->xkey.window == pred.window &&
						event->xkey.serial == pred.serial;
				}
			} predicate(event.xkey.window, event.xkey.serial);

			XEvent pressevent;
			if(XCheckIfEvent(xdisplay, &pressevent, &Predicate::func, reinterpret_cast<XPointer>(&predicate)))
			{
				XPutBackEvent(xdisplay, &pressevent);

				keyrepeat_serial = event.xkey.serial;
				repeat = true;
			}
		}

        /*
		window.impl->eventq.push(
			new wm::KeyEvent(
				window,
				xlib::mapXKeySym(keysym),
				xlib::mapKeyMod(event.xkey.state),
				event.type == KeyPress,
				repeat));
                */
	}

	void EventReader::handleConfigureNotify(wm::Window& window, const XEvent &event)
	{
		if(unsigned(event.xconfigure.width) == width &&
			unsigned(event.xconfigure.height) == height)
			return;

		width = event.xconfigure.width;
		height = event.xconfigure.height;

        /*
		window.impl->eventq.push(
			new wm::ResizeEvent(
				window,
				event.xconfigure.width,
				event.xconfigure.height
				));
                */
	}
}


