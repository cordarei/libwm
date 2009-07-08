#ifndef WM_UTIL_EVENTPRINTER_HPP
#define WM_UTIL_EVENTPRINTER_HPP

#include <string>
#include <ostream>
#include <cctype>

#include <wm/event.hpp>
#include <wm/events.hpp>
#include <wm/eventhandler.hpp>

namespace wm
{
	namespace util
	{
		/// An event handler that prints representations of events to an output stream
		/**
			An example event handler that prints string representations
			of Events to an output stream
		*/
		class EventPrinter : public EventHandler
		{
			public:
				/// Create a new event printer
				/**
					@param os the output stream to print to
					@param delim a delimeter string to print after each event
				*/
				explicit EventPrinter(std::ostream& os, const std::string &delim = "\n")
					: os(&os)
					, delim(delim)
				{
				}

				/// Print an ExposeEvent
				/**
					@param event the event to print
				*/
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

				/// Print a ButtonEvent
				/**
					@param event the event to print
				*/
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

				/// Print a KeyEvent
				/**
					@param event the event to print
				*/
				virtual void handle(const KeyEvent& event)
				{
					typedef unsigned int uint;
					*os
						<< "KeyEvent("
						<< "symbol = " << uint(event.symbol()) << " \"" << keyName(event.symbol()) << '"'
						<< ", state = " << (event.state() ? "Down" : "Up")
						<< ")" << delim;
				}

				/// Print a FocusEvent
				/**
					@param event the event to print
				*/
				virtual void handle(const FocusEvent& event)
				{
					*os
						<< "FocusEvent("
						<< "state = " << (event.state() ? "Got" : "Lost")
						<< ")" << delim;
				}

				/// Print a MouseOverEvent
				/**
					@param event the event to print
				*/
				virtual void handle(const MouseOverEvent& event)
				{
					*os
						<< "MouseOverEvent("
						<< "x = " << event.x()
						<< ", y = " << event.y()
						<< ", inside = " << (event.inside() ? "Inside" : "Outside")
						<< ")" << delim;
				}

				/// Print a ResizeEvent
				/**
					@param event the event to print
				*/
				virtual void handle(const ResizeEvent& event)
				{
					*os
						<< "ResizeEvent("
						<< "width = " << event.width()
						<< ", height = " << event.height()
						<< ")" << delim;
				}

				/// Print a ShowEvent
				/**
					@param event the event to print
				*/
				virtual void handle(const ShowEvent& event)
				{
					*os
						<< "ShowEvent("
						<< "state = " << (event.state() ? "Show" : "Hide")
						<< ")" << delim;
				}

				/// Print a CloseEvent
				/**
					@param event the event to print
				*/
				virtual void handle(const CloseEvent &event)
				{
					(void)event;
					*os
						<< "CloseEvent("
						<< ")" << delim;
				}

				/// Print a CharacterEvent
				/**
					@param event the event to print
				*/
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
	}
}

#endif

