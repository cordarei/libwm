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
	class CharacterEvent;

	class EventHandler
	{
		public:
			virtual ~EventHandler() {}
			
			virtual void handle(const ExposeEvent &) {}
			virtual void handle(const ButtonEvent &) {}
			virtual void handle(const KeyEvent &) {}
			virtual void handle(const FocusEvent &) {}
			virtual void handle(const MouseOverEvent &) {}
			virtual void handle(const ResizeEvent &) {}
			virtual void handle(const ShowEvent &) {}
			virtual void handle(const CloseEvent &) {}
			virtual void handle(const CharacterEvent&) { }
			
		protected:
			EventHandler() {}
	};
}

#endif

