#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <wm/export.hpp>

namespace wm
{
	class Window;
	class Display;

	class WM_EXPORT Context
	{
		public:
			Context(Window &window, int majorVersion, int minorVersion);
			Context(Window &window, Context& sharedContext, int majorVersion, int minorVersion);
			~Context();
			
			Display& display() { return display_; }
			
		private:
			Context(const Context&);
			Context& operator=(const Context&);

			struct impl_t;
			impl_t* impl;
			
			Display &display_;
			friend void WM_EXPORT makeCurrent(Window&, Context&);
	};
}

#undef WM_EXPORT

#endif

