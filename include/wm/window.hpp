#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <wm/pixelformat.hpp>
#include <wm/connection.hpp>

#include <wm/export.hpp>

namespace wm
{
	class Display;
	class Context;
	class EventHandler;
	
	namespace common
	{
		class Dispatcher;
	}

	class WM_EXPORT Window
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
			Window(const Window&);
			Window& operator=(const Window&);

			struct impl_t;
			impl_t* impl;
			Display& display_;

			common::Dispatcher &dispatcher();

			friend class EventReader;
			friend class Connection;
			friend class Display;
			friend class Context;
			friend void WM_EXPORT makeCurrent(Window&, Context&);
	};
}

#undef WM_EXPORT

#endif

