#ifndef XLIB_IMPL_EWMH_HPP
#define XLIB_IMPL_EWMH_HPP

#include <X11/Xlib.h>

namespace wm
{
	namespace xlib
	{
		struct EWMH
		{
			enum  _NET_WM_STATE_ACTION { 
				_NET_WM_STATE_REMOVE = 0,
				_NET_WM_STATE_ADD = 1,
				_NET_WM_STATE_TOGGLE = 2 };

			void init(::Display *xdisplay);
			
			::Display* xdisplay;
			Atom _net_supported;
			Atom _net_wm_state;
			Atom _net_wm_state_fullscreen;			

			void set_wm_state(::Window window, int screen, _NET_WM_STATE_ACTION action, Atom property1, Atom property2) const;
		};
	}
}

#endif

