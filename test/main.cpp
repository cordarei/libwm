#include <iostream>
#include <algorithm>

#include <wm/display.hpp>
#include <wm/window.hpp>
#include <wm/pixelformat.hpp>
#include <wm/context.hpp>

#include <wm/connection.hpp>
#include <wm/eventhandler.hpp>
#include <wm/event.hpp>
#include <wm/ostream_output.hpp>

#include "draw.hpp"

#if defined(WIN32)
#define NOMINMAX // otherwise windows.h macro max will conflict with std::max
#include <windows.h>
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR /* cmdLine */, int)
#else
int main(int, char *[])
#endif
{
	wm::Display display;
	wm::Window window(display, 0, 400, 300);
	
	const unsigned char test_string[] = {
		'w', 'm', ' ', 't', 'e', 's', 't', ' ', ' ',
		'u', 't', 'f', '8', ':', ' ',
		0xc3, 0xa4, 0xc3, 0xa4, 0x6b, 0x6b, 0xc3, 0xb6, 0x73, 0x69, 0xc3, 0xa4, 0x20,
		0xe2, 0x82, 0xac, 0x75, 0x72, 0x6f, 0x6a, 0x61, 0
		};
	
	window.setTitle(reinterpret_cast<const char*>(test_string));
	
	wm::Context context(window, 1, 2);
	makeCurrent(window, context);

	struct Handler : public wm::EventHandler
	{
		explicit Handler(wm::Window &window) : window(&window), quit_flag(false)
		{
			// TODO: query these from window
			window.getSize(width, height);
			std::cout << "window size: " << width << "x" << height << std::endl; 
		}
	
		virtual void handle(const wm::ExposeEvent &event)
		{
			test::draw(width, height);
			window->swap();
		}
		
		virtual void handle(const wm::ButtonEvent &event)
		{			
			if(event.state())
			{
				unsigned int w, h;
				window->getSize(w, h);
				std::cout << "window old size: " << w << "x" << h << std::endl; 
				window->resize(w+100, h+100);
			}
		}

		virtual void handle(const wm::ResizeEvent &event)
		{
			width = event.width();
			height = event.height();
		}
				
		virtual void handle(const wm::CloseEvent&)
		{
			quit_flag = true;
		}

		wm::Window *window;
		unsigned int width, height;
		bool quit_flag;
	} handler(window);

	wm::EventPrinter printer(std::cout);
	wm::Connection printerconn(window, printer);

	wm::Connection connection(window, handler);

	window.show();
	window.swap();

	while(!handler.quit_flag) window.dispatch(true);

	return 0;
}

