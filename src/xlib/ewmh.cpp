#include <cstring>
#include <map>

#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include <wm/exception.hpp>

#include <xlib/impl/ewmh.hpp>

namespace wm
{
	namespace xlib
	{
		void EWMH::init(::Display *xdisplay, int screen)
		{
			this->xdisplay = xdisplay;
			this->screen = screen;

			static const struct AtomMapping
			{
				AtomMapping()
				{
					map["_NET_WM_STATE"] = &EWMH::_net_wm_state;
					map["_NET_WM_STATE_FULLSCREEN"] = &EWMH::_net_wm_state_fullscreen;
				}
			
				typedef std::map<std::string, Atom EWMH::*> map_t;
				map_t map;
			} mapping;
			
			for(AtomMapping::map_t::const_iterator iter = mapping.map.begin(); 
				iter != mapping.map.end();
				++iter)
			{
				Atom EWMH::*mem = iter->second;
				this->*mem = 0;
			}
			
			Window root = RootWindow(xdisplay, screen);	 // TODO: make this work with multiple X screens

			_net_supported = XInternAtom(xdisplay, "_NET_SUPPORTED", False);

			Atom type = XA_ATOM;
			Atom actual_type;
			int actual_format;
			unsigned long nitems;
			unsigned long bytes_after;
			unsigned char *data = 0;
			
			long offset = 0;
			long count = 0;
			
			for(int i = 0; i < 2; ++i)
			{
				XFree(data);
				data = 0;
			
				int ret = XGetWindowProperty(xdisplay, root,
					_net_supported,
					offset, count, False,
					type,
					&actual_type, &actual_format, &nitems, &bytes_after, &data);
			
				if(ret != Success)
				{
					XFree(data);
					throw wm::Exception("Can't initialize extended window manager hints: XGetWindowProperty failed");
				}
			
				if(actual_type == None)
				{
					_net_supported = 0;
					XFree(data);
					break;
				}
				
				if(bytes_after > 0)
					count = bytes_after / 4;
			}
			
			Atom *atomdata = reinterpret_cast<Atom*>(data);
			for(Atom *ptr = atomdata+0; ptr != atomdata+nitems; ++ptr)
			{
				Atom atom = *ptr;
				char *atomname = XGetAtomName(xdisplay, atom);

				AtomMapping::map_t::const_iterator iter = mapping.map.find(atomname);
				
				XFree(atomname);

				if(iter != mapping.map.end())
				{
					Atom EWMH::*mem = iter->second;
					this->*mem = atom;
				}
			}
			
			XFree(data);
		}
		
		void EWMH::set_wm_state(::Window window, _NET_WM_STATE_ACTION action, Atom property1, Atom property2) const
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
	}
}
