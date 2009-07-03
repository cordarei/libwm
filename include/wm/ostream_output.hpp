#ifndef WM_OSTREAM_OUTPUT_HPP
#define WM_OSTREAM_OUTPUT_HPP

#include <iostream>
#include <string>
#include <cctype>

#include <wm/event.hpp>
#include <wm/events.hpp>
#include <wm/eventhandler.hpp>
#include <wm/pixelformat.hpp>
#include <wm/configuration.hpp>

namespace wm
{
	class EventPrinter : public EventHandler
	{
		public:
			explicit EventPrinter(std::ostream& os, const std::string &delim = "\n")
				: os(&os)
				, delim(delim)
			{
			}

			virtual void handle(const ExposeEvent& event)
			{
				*os
					<< "ExposeEvent("
					<< "x = " << event.x()
					<< ", y = " << event.y()
					<< ", width = " << event.width()
					<< ", height = " << event.height()
					<< ")" << delim;
			}

			virtual void handle(const ButtonEvent& event)
			{
				*os
					<< "ButtonEvent("
					<< "x = " << event.x()
					<< ", y = " << event.y()
					<< ", button = " << event.button()
					<< ", state = " << (event.state() ? "Down" : "Up")
					<< ")" << delim;
			}

			virtual void handle(const KeyEvent& event)
			{
				typedef unsigned int uint;
				*os
					<< "KeyEvent("
					<< "symbol = " << uint(event.symbol()) << " \"" << keyName(event.symbol()) << '"'
					<< ", state = " << (event.state() ? "Down" : "Up")
					<< ")" << delim;
			}

			virtual void handle(const FocusEvent& event)
			{
				*os
					<< "FocusEvent("
					<< "state = " << (event.state() ? "Got" : "Lost")
					<< ")" << delim;
			}

			virtual void handle(const MouseOverEvent& event)
			{
				*os
					<< "MouseOverEvent("
					<< "x = " << event.x()
					<< ", y = " << event.y()
					<< ", inside = " << (event.inside() ? "Inside" : "Outside")
					<< ")" << delim;
			}

			virtual void handle(const ResizeEvent& event)
			{
				*os
					<< "ResizeEvent("
					<< "width = " << event.width()
					<< ", height = " << event.height()
					<< ")" << delim;
			}

			virtual void handle(const ShowEvent& event)
			{
				*os
					<< "ShowEvent("
					<< "state = " << (event.state() ? "Show" : "Hide")
					<< ")" << delim;
			}

			virtual void handle(const CloseEvent &)
			{
				*os
					<< "CloseEvent("
					<< ")" << delim;
			}

			virtual void handle(const CharacterEvent& event)
			{
				*os
					<< "CharacterEvent("
					<< "unicode = " << event.unicode();

				int (*func)(int) = &std::isprint; // use the C isprint, not the C++ one with locales
				char ch = event.unicode() < 128 ? static_cast<char>(event.unicode()) : 0;
				if(func(ch))
					std::cout << " '" << ch << "'";

				*os << ")" << delim;
			}

		private:
			std::ostream* os;
			std::string delim;
	};

	std::ostream& operator<<(std::ostream& os, const Event& event)
	{
		EventPrinter handler(os, "");
		event.accept(handler);
		return os;	
	}
	
	std::ostream& operator<<(std::ostream& os, const PixelFormat& format)
	{
		return os << "PixelFormat"
			<< "  r: " << format.red()
			<< "  g: " << format.green()
			<< "  b: " << format.blue()
			<< "  a: " << format.alpha()
			<< "  depth: " << format.depth()
			<< "  stencil: " << format.stencil()
			;
	}
	
	std::ostream& operator<<(std::ostream& os, const Configuration& config)
	{
		int num = config.numFormats();
		os << num << " pixel formats" << std::endl;
		
		for(int i = 0; i < num; ++i)
		{
			os << config.getFormat(i) << std::endl;
		}
		
		return os;
	}	
}

#endif
