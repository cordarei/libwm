#ifndef DISPLAY_IMPL_HPP
#define DISPLAY_IMPL_HPP

#include <map>

#include <X11/Xlib.h>

#include <wm/display.hpp>

#include <xlib/impl/ewmh.hpp>

namespace wm
{
	struct Display::impl_t
	{
		// Display handle
		::Display *display;
		int screen;
		
		// WM_DELETE_WINDOW atom for handling window closing
		Atom wm_delete_window;
		
		// Extended windom manager hints
		xlib::EWMH ewmh;
		
		// X input method
		XIM xim;
		
		// Xlib window handle to wm::Window* mapping
		class Registry
		{
			public:
				void add(::Window handle, wm::Window* ptr)
				{
					using std::make_pair;
					map.insert(make_pair(handle, ptr));
				}
				
				void remove(::Window handle)
				{
					map_t::iterator i = map.find(handle);
					if(i != map.end()) map.erase(i);
				}

				wm::Window* get(::Window handle)
				{
					map_t::const_iterator i = map.find(handle);
					if(i != map.end()) return i->second;
					return 0;
				}				

			private:
				typedef ::Window Handle;
				typedef std::map<Handle, wm::Window*> map_t;
				map_t map;
		} registry;
	};
}

#endif

