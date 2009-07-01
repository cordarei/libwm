#ifndef COMMON_DISPATCHER_HPP
#define COMMON_DISPATCHER_HPP

namespace wm
{
	class Event;
	class EventHandler;

	namespace common
	{
		struct ConnectionInfo;
	
		class Dispatcher
		{		
			public:
				Dispatcher();
				~Dispatcher();
			
				ConnectionInfo* connect(EventHandler&);
				void disconnect(ConnectionInfo*);
				
				void dispatch(const Event&);
				
			private:
				Dispatcher(const Dispatcher&);
				Dispatcher& operator=(const Dispatcher&);

				ConnectionInfo *head;
		};
	}
}

#endif

