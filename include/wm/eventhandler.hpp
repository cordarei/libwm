#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP

namespace wm
{
	class EventHandler
	{
		public:
			virtual ~EventHandler() {}
			
			virtual void expose(
				unsigned int x,
				unsigned int y,
				unsigned int width,
				unsigned int height) = 0;
			
		protected:
			EventHandler() {}
	};
}

#endif

