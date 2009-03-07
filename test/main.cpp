#include <iostream>

#include <wm/display.hpp>
#include <wm/window.hpp>
#include <wm/pixelformat.hpp>
#include <wm/context.hpp>

#include <wm/connection.hpp>
#include <wm/eventhandler.hpp>
#include <wm/event.hpp>

#include "draw.hpp"

#ifdef WIN32
#include <windows.h>
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR /* cmdLine */, int)
#else
int main(int, char *[])
#endif
{
	wm::Display display;
	wm::Window window(display, 0, 400, 300);
	wm::Context context(window, 1, 2);
	makeCurrent(window, context);

	struct Handler : public wm::EventHandler
	{
		explicit Handler(wm::Window &window) : window(&window), quit_flag(false)
		{
			// TODO: query these from window
			width = 400;
			height = 300;
		}
	
		virtual void handle(const wm::ExposeEvent &event)
		{
			std::cout
				<< "expose"
				<< ": " << event.x()
				<< ", " << event.y()
				<< ", " << event.width()
				<< ", " << event.height()
				<< std::endl;

			width = std::max(width, event.width());
			height = std::max(height, event.height());
			
			test::draw(width, height);
			window->swap();
		}
		
		virtual void handle(const wm::ButtonEvent &event)
		{
			std::cout 
				<< "button"
				<< " " << event.button()
				<< " " << (event.state()?"down":"up")
				<< "  x: " << event.x()
				<< "  y: " << event.y()
				<< std::endl;
		}
		
		virtual void handle(const wm::KeyEvent &event)
		{
			std::cout
				<< "key"
				<< " " << (event.state()?"down":"up")
				<< std::endl;
		}

		virtual void handle(const wm::MouseOverEvent &event)
		{
			std::cout
				<< "mouse "
				<< " " << (event.inside()?"enter":"exit")
				<< "  x: " << event.x()
				<< "  y: " << event.y()
				<< std::endl;
		}
		
		virtual void handle(const wm::FocusEvent &event)
		{
			std::cout
				<< (event.state()?"got":"lost")
				<< " focus"
				<< std::endl;
		}

		virtual void handle(const wm::ResizeEvent &event)
		{
			width = event.width();
			height = event.height();
		
			std::cout
				<< "resize"
				<< "  width: " << event.width()
				<< "  height: " << event.height()
				<< std::endl;
		}
		
		virtual void handle(const wm::ShowEvent &event)
		{
			std::cout
				<< (event.state()?"show":"hide")
				<< " window"
				<< std::endl;
		}
		
		virtual void handle(const wm::CloseEvent&)
		{
			std::cout << "close window" << std::endl;
			quit_flag = true;
		}

		wm::Window *window;
		unsigned int width, height;
		bool quit_flag;
	} handler(window);

	wm::Connection connection(window, handler);

	window.show();
	window.swap();

	while(!handler.quit_flag) window.dispatch(true);
	
	return 0;
}

