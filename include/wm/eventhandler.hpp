#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP

namespace wm
{
	class ExposeEvent;
	class ButtonEvent;
	class KeyEvent;
	class FocusEvent;
	class MouseOverEvent;
	class ResizeEvent;
	class ShowEvent;
	class CloseEvent;

	class EventHandler
	{
		public:
			virtual ~EventHandler() {}
			
			virtual void handle(const ExposeEvent &) = 0;
			virtual void handle(const ButtonEvent &) = 0;
			virtual void handle(const KeyEvent &) = 0;
			virtual void handle(const FocusEvent &) = 0;
			virtual void handle(const MouseOverEvent &) = 0;
			virtual void handle(const ResizeEvent &) = 0;
			virtual void handle(const ShowEvent &) = 0;
			virtual void handle(const CloseEvent &) = 0;
			
		protected:
			EventHandler() {}
	};
}

#endif

