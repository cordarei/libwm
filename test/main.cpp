#include <iostream>
#include <algorithm>
#include <string>
#include <stdexcept>

#include <wm/display.hpp>
#include <wm/window.hpp>
#include <wm/configuration.hpp>
#include <wm/pixelformat.hpp>
#include <wm/context.hpp>
#include <wm/surface.hpp>

#include <wm/connection.hpp>
#include <wm/eventhandler.hpp>
#include <wm/event.hpp>
#include <wm/events.hpp>
#include <wm/ostream_output.hpp>

#include "draw.hpp"

namespace
{
	int encode_utf8(boost::uint32_t codepoint, unsigned char *out)
	{
		typedef unsigned char uchar;
	
		if(!(codepoint & ~0x7F))
		{
			out[0] = uchar(codepoint & 0x7F);
			return 1;
		} else if(!(codepoint & ~0x7FF))
		{
			out[0] = uchar(0xc0 | (codepoint & 0x7C0) >> 6);
			out[1] = uchar(0x80 | (codepoint & 0x3F));
			return 2;
		} else if(!(codepoint & ~0xFFFF))
		{
			out[0] = uchar(0xe0 | (codepoint & 0xF000) >> 12);
			out[1] = uchar(0x80 | (codepoint & 0xFC) >> 6);
			out[2] = uchar(0x80 | (codepoint & 0x3F));
			return 3;
		} else if(!(codepoint & ~0x3FFFF))
		{
			out[0] = uchar(0xf0 | (codepoint & 0x1c0000) >> 18);
			out[1] = uchar(0x80 | (codepoint & 0x3f00) >> 12);
			out[2] = uchar(0x80 | (codepoint & 0xFC) >> 6);
			out[3] = uchar(0x80 | (codepoint & 0x3F));
			return 4;
		}
		
		throw std::runtime_error("Invalid Unicode codepoint");
	}
	
	std::string encode_utf8(boost::uint32_t codepoint)
	{
		unsigned char buf[4];
		int len = encode_utf8(codepoint, buf);
		return std::string(
			reinterpret_cast<char*>(buf+0),
			reinterpret_cast<char*>(buf+len)
			);
	}
}

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
	
	wm::Configuration config(window);
	wm::PixelFormat format = config.getFormat(0);
	
	wm::Surface surface(window, format);
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
			if(event.symbol() == wm::KeyEvent::ESCAPE &&
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
			else title_string += encode_utf8(event.unicode());
			window->setTitle(title_string.c_str());
		}

		wm::Window *window;
		bool quit_flag;
		std::string title_string;
		unsigned int width, height;
	} handler(window);

	wm::EventPrinter printer(std::cout);
	wm::Connection printerconn(window, printer);

	wm::Connection connection(window, handler);

	window.show();
	makeCurrent(context, surface, surface);
	
	surface.swap();
	
	while(!handler.quit_flag) window.dispatch(true);

	return 0;
}

