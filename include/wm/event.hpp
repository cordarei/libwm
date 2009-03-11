#ifndef EVENT_HPP
#define EVENT_HPP

namespace wm
{
	class Window;
	class EventHandler;

	class Event
	{
		public:
			virtual ~Event() {}
			
			virtual void accept(EventHandler& handler) const = 0;
			
			Window &window() { return *window_; }
			const Window &window() const { return *window_; }
			
		protected:
			explicit Event(Window& window) : window_(&window) {}
		private:
			Window *window_;
	};
}

#include <boost/cstdint.hpp>
#include <wm/eventhandler.hpp>

namespace wm
{	
	class ExposeEvent : public Event
	{
		public:
			ExposeEvent(
				Window& window,
				unsigned int x,
				unsigned int y,
				unsigned int width,
				unsigned int height)
				: Event(window)
				, x_(x)
				, y_(y)
				, width_(width)
				, height_(height)
			{
			}
			
			unsigned int x() const { return x_; }
			unsigned int y() const { return y_; }
			unsigned int width() const { return width_; }
			unsigned int height() const { return height_; }
			
			virtual void accept(EventHandler &handler) const { handler.handle(*this); }
			
		private:
			unsigned int x_, y_, width_, height_;
	};
	
	class ButtonEvent : public Event
	{
		public:
			ButtonEvent(
				Window& window,
				unsigned int x,
				unsigned int y,
				int button,
				bool state)
				: Event(window)
				, x_(x)
				, y_(y)
				, button_(button)
				, state_(state)
			{
			}
			
			unsigned int x() const { return x_; }
			unsigned int y() const { return y_; }
			int button() const { return button_; }
			bool state() const { return state_; }
			
			virtual void accept(EventHandler &handler) const { handler.handle(*this); }
		
		private:
			unsigned int x_, y_;
			int button_;
			bool state_;
	};
	
	class KeyEvent : public Event
	{
		public:
			KeyEvent(
				Window &window,
				bool state)
				: Event(window)
				, state_(state)
			{
			}

			bool state() const { return state_; }

			virtual void accept(EventHandler &handler) const { handler.handle(*this); }
			
		private:
			bool state_;
	};

	class FocusEvent : public Event
	{
		public:
			FocusEvent(
				Window &window,
				bool state)
				: Event(window)
				, state_(state)
			{
			}

			bool state() const { return state_; }

			virtual void accept(EventHandler &handler) const { handler.handle(*this); }
			
		private:
			bool state_;
	};

	class MouseOverEvent : public Event
	{
		public:
			MouseOverEvent(
				Window &window,
				int x,
				int y,
				bool inside)
				: Event(window)
				, x_(x)
				, y_(y)
				, inside_(inside)
			{
			}

			int x() const { return x_; }
			int y() const { return y_; }
			bool inside() const { return inside_; }

			virtual void accept(EventHandler &handler) const { handler.handle(*this); }
			
		private:
			int x_, y_;
			bool inside_;
	};

	class ResizeEvent : public Event
	{
		public:
			ResizeEvent(
				Window& window,
				unsigned int width,
				unsigned int height)
				: Event(window)
				, width_(width)
				, height_(height)
			{
			}
			
			unsigned int width() const { return width_; }
			unsigned int height() const { return height_; }
			
			virtual void accept(EventHandler &handler) const { handler.handle(*this); }
			
		private:
			unsigned int width_, height_;
	};

	class ShowEvent : public Event
	{
		public:
			ShowEvent(
				Window &window,
				bool state)
				: Event(window)
				, state_(state)
			{
			}

			bool state() const { return state_; }

			virtual void accept(EventHandler &handler) const { handler.handle(*this); }
			
		private:
			bool state_;
	};

	class CloseEvent : public Event
	{
		public:
			CloseEvent(
				Window &window)
				: Event(window)
			{
			}

			virtual void accept(EventHandler &handler) const { handler.handle(*this); }
	};

	class CharacterEvent : public Event
	{
		public:
			CharacterEvent(
				Window &window,
				boost::uint32_t unicode)
				: Event(window)
				, unicode_(unicode)
			{
			}

			virtual void accept(EventHandler &handler) const { handler.handle(*this); }

			boost::uint32_t unicode() const { return unicode_; }


		private:
			boost::uint32_t unicode_;
	};

}

#endif

