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
			explicit Context(Window& window, const PixelFormat &format, Context *shared = 0);
			~Context();
			
			Display& display() { return display_; }
			
		private:
			Context(const Context&);
			Context& operator=(const Context&);

			struct impl_t;
			impl_t* impl;
			
			Display &display_;

			friend void WM_EXPORT makeCurrent(Context &context, Surface &draw, Surface &read);
	};
	
	void WM_EXPORT makeCurrent(Context &context, Surface &draw, Surface &read);
}

#undef WM_EXPORT

#endif

