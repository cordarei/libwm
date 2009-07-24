#include <cstring>

#include <X11/Xlib.h>

#include <wm/exception.hpp>

#include <xlib/impl/ewmh.hpp>

namespace wm
{
	namespace xlib
	{
		void EWMH::init(::Display *xdisplay)
		{
			this->xdisplay = xdisplay;
		
			const struct AtomRef
			{
				const char *name;
				Atom *atom;
			} refs[] = {
				{ "_NET_SUPPORTED", &_net_supported },
				{ "_NET_WM_STATE", &_net_wm_state },
				{ "_NET_WM_STATE_FULLSCREEN", &_net_wm_state_fullscreen }
				};
				
			const int num_atoms = sizeof(refs) / sizeof(*refs);
			
			char *names[num_atoms];
			Atom atoms[num_atoms];
			
			for(int i = 0; i < num_atoms; ++i)
				names[i] = const_cast<char*>(refs[i].name);
		
			if(!XInternAtoms(xdisplay, names, num_atoms, False, atoms))
				throw wm::Exception("Can't initialize Extended Window Manager Hints: XInternAtoms failed");
				
			for(int i = 0; i < num_atoms; ++i)
				*(refs[i].atom) = atoms[i];
		
		}
		
		void EWMH::set_wm_state(::Window window, int screen, _NET_WM_STATE_ACTION action, Atom property1, Atom property2) const
		{
			XEvent event;
			std::memset(&event, 0, sizeof(XEvent));
		
			event.xclient.type = ClientMessage;
			event.xclient.message_type = _net_wm_state;
			event.xclient.display = xdisplay;
			event.xclient.window = window;
			event.xclient.format = 32;
			event.xclient.data.l[0] = action;
			event.xclient.data.l[1] = property1;
			event.xclient.data.l[2] = property2;
			event.xclient.data.l[3] = 1;

			XSendEvent(
				xdisplay,
				RootWindow(xdisplay, screen),
				False,
				(SubstructureNotifyMask|SubstructureRedirectMask),
				&event);		
		}
		
		void EWMH::set_fullscreen(::Window window, int screen, _NET_WM_STATE_ACTION action) const
		{
			set_wm_state(window, screen, action, _net_wm_state_fullscreen, 0);
		}
	}
}
