#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include <wm/pixelformat.hpp>
#include <wm/connection.hpp>

namespace wm
{
	class Display;
	class Context;
	class EventHandler;
	
	namespace common
	{
		class Dispatcher;
	}

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
			
			void swap();
			
			Display& display() { return display_; }
			
			void dispatch(bool block);

			common::Dispatcher &dispatcher();
			
		private:
			struct impl_t;
			boost::scoped_ptr<impl_t> impl;
			Display& display_;
			
			friend class Display;
			friend class Context;
			friend void makeCurrent(Window&, Context&);
	};
}

#endif

