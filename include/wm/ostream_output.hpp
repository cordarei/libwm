#ifndef WM_OSTREAM_OUTPUT_HPP
#define WM_OSTREAM_OUTPUT_HPP

#include <iostream>
#include <string>

#include <wm/event.hpp>
#include <wm/eventhandler.hpp>

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
				*os
					<< "KeyEvent("
					<< "state = " << (event.state() ? "Down" : "Up")
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
					<< "ResizeEvent("
					<< "state = " << (event.state() ? "Show" : "Hide")
					<< ")" << delim;
			}

			virtual void handle(const CloseEvent &)
			{
				*os
					<< "CloseEvent("
					<< ")" << delim;
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
}

#endif