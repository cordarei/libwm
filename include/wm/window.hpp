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
			
			void getSize(unsigned int &width, unsigned int &height);
			void resize(unsigned int width, unsigned int height);
			
			void setTitle(const char* title); // utf-8
			
			void swap();
			
			Display& display() { return display_; }
			
			void dispatch(bool block);
			
		private:
			struct impl_t;
			boost::scoped_ptr<impl_t> impl;
			Display& display_;

			common::Dispatcher &dispatcher();

			friend class EventReader;
			friend class Connection;
			friend class Display;
			friend class Context;
			friend void makeCurrent(Window&, Context&);
	};
}

#endif

