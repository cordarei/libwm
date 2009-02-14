#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include <wm/pixelformat.hpp>

namespace wm
{
	class Display;

	class Window : boost::noncopyable
	{
		public:
			Window(
				Display& display,
				int screen,
				unsigned int width,
				unsigned int height,
				const PixelFormat& format = PixelFormat());
			
			~Window();
				
			void show();
			void hide();
			
			Display& display() { return display_; }
			
		private:
			struct impl_t;
			boost::scoped_ptr<impl_t> impl;
			
			Display& display_;
	};
}

#endif

