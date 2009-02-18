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
			
		private:
			struct impl_t;
			boost::scoped_ptr<impl_t> impl;
			Display& display_;
			
			class ConnectionInfo;
			void connect(EventHandler &, ConnectionInfo &);
			void disconnect(ConnectionInfo &);
			bool isConnected(ConnectionInfo &);
			
			friend class Connection;
			friend struct Connection::impl_t;
			friend class Context;
			friend void makeCurrent(Window&, Context&);
	};
}

#endif

