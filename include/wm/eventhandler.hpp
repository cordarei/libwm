#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP

namespace wm
{
	class ExposeEvent;

	class EventHandler
	{
		public:
			virtual ~EventHandler() {}
			
			virtual void handle(const ExposeEvent &event) = 0;
			
		protected:
			EventHandler() {}
	};
}

#endif

