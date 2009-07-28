#ifndef WM_UTIL_EVENTPRINTER_HPP
#define WM_UTIL_EVENTPRINTER_HPP

#include <string>
#include <ostream>
#include <cctype>

#include <wm/event.hpp>
#include <wm/events.hpp>
#include <wm/eventhandler.hpp>
#include <wm/keyboard.hpp>

namespace wm
{
	namespace util
	{
		namespace detail
		{
			std::ostream &printKeyMod(std::ostream& os, wm::keyboard::KeyMod mod)
			{
				return os << ""
					<< ((mod & wm::keyboard::MOD_SHIFT) ? "Shift " : "")
					<< ((mod & wm::keyboard::MOD_CONTROL) ? "Control " : "")
					<< ((mod & wm::keyboard::MOD_ALT) ? "Alt " : "")
					<< ((mod & wm::keyboard::MOD_SUPER) ? "Super " : "")
					<< ((mod & wm::keyboard::MOD_ALTGR) ? "AltGr " : "")
					<< ((mod & wm::keyboard::MOD_NUMLOCK) ? "Num Lock " : "")
					<< ((mod & wm::keyboard::MOD_CAPSLOCK) ? "Caps Lock " : "")
					<< ((!mod) ? "None " : "")
					;
			}
		
			std::ostream &printButtonMask(std::ostream &os, wm::mouse::ButtonMask mask)
			{
				return os << ""
					<< ((mask & wm::mouse::MASK_LEFT) ? "Left " : "")
					<< ((mask & wm::mouse::MASK_MIDDLE) ? "Middle " : "")
					<< ((mask & wm::mouse::MASK_RIGHT) ? "Right " : "")
					<< ((!mask) ? "None " : "")
					;		
			}
		}
	
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
						<< ", button = " << mouse::buttonName(event.button())
						<< ", state = " << (event.state() ? "Down" : "Up")
						<< ", buttons = ";
					detail::printButtonMask(*os, event.buttons());
					*os << ", keymod = ";
					detail::printKeyMod(*os, event.keymod());
					*os << ")" << delim;
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
						<< "symbol = " << uint(event.symbol())
						<< " \"" << keyboard::keyName(event.symbol()) << '"'
						<< ", state = " << (event.state() ? "Down" : "Up")
						<< ", repeat = " << (event.repeat() ? "true" : "false")
						<< ", keymod = ";
					detail::printKeyMod(*os, event.keymod());	
					*os	<< ")" << delim;
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

				/// Print a MotionEvent
				/**
					@param event the event to print
				*/
				virtual void handle(const MotionEvent& event)
				{
					*os
						<< "MotionEvent("
						<< "x = " << event.x()
						<< ", y = " << event.y()
						<< ", buttons = ";
					detail::printButtonMask(*os, event.buttons());
					*os << ", keymod = ";
					detail::printKeyMod(*os, event.keymod());
					*os << ")" << delim;
				}
			private:
				std::ostream* os;
				std::string delim;
		};
	}
}

#endif

