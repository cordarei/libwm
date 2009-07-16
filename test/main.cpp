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
	const wm::PixelFormat& format =
		choose(config, wm::PixelFormat::Descriptor(8, 8, 8, 8, 16, 0));

	wm::Window window(display, 0, 400, 300, format);

	wm::Surface surface(window);
	wm::Context context(format);

	struct Handler : public wm::EventHandler
	{
		explicit Handler(wm::Window &window) : window(&window), quit_flag(false), title_string("wm test")
		{
			window.getSize(width, height);
			window.setTitle(title_string.c_str());
		}
	
		virtual void handle(const wm::ExposeEvent &event)
		{
			test::draw(width, height);
			event.window().surface().swap();
		}

		virtual void handle(const wm::KeyEvent &event)
		{
			if(event.symbol() == wm::keyboard::ESCAPE &&
				event.state())
			{
				quit_flag = true;
			}
		};
		
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
		
		virtual void handle(const wm::CharacterEvent& event)
		{
			if(event.unicode() == '\n' || event.unicode() == '\r') title_string.clear();
			else title_string += wm::util::encode_utf8(event.unicode());
			window->setTitle(title_string.c_str());
		}

		wm::Window *window;
		bool quit_flag;
		std::string title_string;
		unsigned int width, height;
	} handler(window);

	wm::util::EventPrinter printer(std::cout);
	wm::Connection printerconn(window, printer);

	wm::Connection connection(window, handler);

	window.show();
	makeCurrent(context, surface, surface);
	
	surface.swap();
	
	while(!handler.quit_flag) window.dispatch(true);

	return 0;
}

