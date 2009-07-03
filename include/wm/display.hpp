#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <wm/export.hpp>

namespace wm
{
	class Window;
	class Context;
	class Surface;

	class WM_EXPORT Display
	{
		public:
			explicit Display(const char *name = 0);
			~Display();
			
			void dispatch(bool block);
			
		private:
			Display(const Display&);
			Display& operator=(const Display&);

			struct impl_t;
			impl_t* impl;
			
			friend class EventReader;
			friend class Window;
			friend class Context;
			friend class Configuration;
			friend class Surface;
			
			friend void WM_EXPORT makeCurrent(Context &context, Surface &draw, Surface &read);
	};
}

#undef WM_EXPORT

#endif


