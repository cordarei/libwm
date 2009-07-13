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
	class MotionEvent;

	/// An event handler
	/**
		@see Connection
		@see Event
	*/
	class EventHandler
	{
		public:
			/// Destruct an event handler
			virtual ~EventHandler() {}
			
			/// Handle an expose event
			/**
				@param event the ExposeEvent event object
			*/
			virtual void handle(const ExposeEvent &event) {}
			
			/// Handle a mouse button event
			/**
				@param event the ButtonEvent event object
			*/
			virtual void handle(const ButtonEvent &event) {}
			
			/// Handle a keyboard event
			/**
				@param event the KeyEvent event object
			*/
			virtual void handle(const KeyEvent &event) {}
			
			/// Handle a keyboard focus event
			/**
				@param event the FocusEvent event object
			*/
			virtual void handle(const FocusEvent &event) {}
			
			/// Handle a mouse over event event
			/**
				@param event the MouseOverEvent event object
			*/
			virtual void handle(const MouseOverEvent &event) {}
			
			/// Handle a window resizing event
			/**
				@param event the ResizeEvent event object
				@note you should not redraw the window in the resize event handler
			*/
			virtual void handle(const ResizeEvent &event) {}
			
			/// Handle a show event
			/**
				@param event the ShowEvent event object
			*/
			virtual void handle(const ShowEvent &event) {}
			
			/// Handle a window closing event
			/**
				@param event the CloseEvent event object
			*/
			virtual void handle(const CloseEvent &event) {}
			
			/// Handle a text input event
			/**
				@param event the CharacterEvent event object
			*/
			virtual void handle(const CharacterEvent &event) { }

			/// Handle a mouse motion event
			/**
				@param event the MotionEvent event object
			*/
			virtual void handle(const MotionEvent &event) { }
			
		protected:
			EventHandler() {}
	};
}

#endif

