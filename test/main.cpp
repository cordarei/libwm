#include <iostream>
#include <algorithm>
#include <string>
#include <stdexcept>

#include <wm/wm.hpp>
#include <wm/util/eventprinter.hpp>
#include <wm/util/ostream_output.hpp>
#include <wm/util/utf8.hpp>

#include "draw.hpp"

int wm_main(int argc, char *argv[])
{
	wm::Display display;
	
	wm::Configuration config(display);
	wm::Window window(display, 400, 300, choose(config, 8, 8, 8, 8, 16, 0));
	wm::Surface surface(window);
	wm::Context context(window.pixelformat());

	struct Handler : public wm::EventHandler
	{
		explicit Handler(wm::Window &window) : window(&window), quit_flag(false), title_string("wm test")
		{
			window.setTitle(title_string.c_str());
			fullscreen = false;
			resizable = true;
			flipped = false;
		}
	
		virtual void handle(const wm::ExposeEvent &event)
		{
			unsigned int width, height;
			event.window().surface().getSize(width, height);

			test::draw(width, height, flipped);
			event.window().surface().swap();
		}

		virtual void handle(const wm::KeyEvent &event)
		{
			if(event.symbol() == wm::keyboard::ESCAPE &&
				event.state())
			{
				quit_flag = true;
			}
			
			if(event.state() && event.symbol() == wm::keyboard::F11)
			{
				// this does not work if fullscreen does not actually switch
				// because of size limits
				fullscreen = !fullscreen;
				event.window().fullscreen(fullscreen);
			}
			
			if(event.state() && event.symbol() == wm::keyboard::SPACE)
			{
				resizable = !resizable;
				if(resizable) event.window().setMinMaxSize(0, 0, 0, 0);
				else
				{
					event.window().setMinMaxSize(200, 200, 200, 200);
					event.window().resize(200, 200);
				}
			}
			
			if(event.state() && event.symbol() == wm::keyboard::RETURN)
			{
				flipped = !flipped;
				event.window().repaint();
			}
		};
		
		virtual void handle(const wm::ButtonEvent &event)
		{			
			if(event.state() && event.button() == wm::mouse::BUTTON_LEFT)
			{
				unsigned int w, h;
				window->getSize(w, h);
				std::cout << "window old size: " << w << "x" << h << std::endl; 
				window->resize(w+100, h+100);
			}
			
			if(event.button() == wm::mouse::BUTTON_RIGHT)
			{
				if(!event.state())
				{
					unsigned int w, h;
					event.window().getSize(w, h);
					event.window().warpMouse(w / 2, h / 2);
				}

				event.window().showCursor(!event.state());
			}
		}

		virtual void handle(const wm::CloseEvent&)
		{
			quit_flag = true;
		}
		
		virtual void handle(const wm::CharacterEvent& event)
		{
			if(event.unicode() == '\n' || event.unicode() == '\r') title_string.clear();
			else title_string += wm::util::encode_utf8(event.unicode());
			window->setTitle(title_string.c_str());
		}

		wm::Window *window;
		bool quit_flag;
		std::string title_string;
		bool fullscreen;
		bool resizable;
		bool flipped;
	} handler(window);

	wm::util::EventPrinter printer(std::cout);
	wm::Connection printerconn(window, printer);

	wm::Connection connection(window, handler);

	window.show();
	makeCurrent(context, surface, surface);
	
	int min, maj;
	test::checkGLversion(maj, min);
	std::cout << "GL version " << maj << '.' << min << std::endl;
	
	surface.swap();
	
	while(!handler.quit_flag) window.wait();

	return 0;
}

