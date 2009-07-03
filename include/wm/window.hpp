#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <wm/pixelformat.hpp>
#include <wm/connection.hpp>
#include <wm/exception.hpp>

#include <wm/export.hpp>

namespace wm
{
	class Display;
	class EventHandler;
	class Surface;
	
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
				unsigned int height);
			
			~Window();
				
			void show();
			void hide();
			
			void getSize(unsigned int &width, unsigned int &height);
			void resize(unsigned int width, unsigned int height);
			
			void setTitle(const char* title); // utf-8
			
			Display& display() { return display_; }

			Surface& surface() const
			{
				if(!surface_) throw wm::Exception("No Surface attached to Window");
				return *surface_;
			}
			
			void dispatch(bool block);

		private:
			Window(const Window&);
			Window& operator=(const Window&);

			struct impl_t;
			impl_t* impl;
			Display& display_;
			Surface *surface_;

			common::Dispatcher &dispatcher();

			friend class EventReader;
			friend class Connection;
			friend class Display;
			friend class Context;
			friend class Configuration;
			friend class Surface;
	};
}

#undef WM_EXPORT

#endif

