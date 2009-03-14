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

#endif

