#ifndef COMMON_DISPATCHER_HPP
#define COMMON_DISPATCHER_HPP

#include <boost/noncopyable.hpp>

namespace wm
{
	class Event;
	class EventHandler;

	namespace common
	{
		struct ConnectionInfo;
	
		class Dispatcher : boost::noncopyable
		{		
			public:
				Dispatcher();
				~Dispatcher();
			
				ConnectionInfo* connect(EventHandler&);
				void disconnect(ConnectionInfo*);
				
				void dispatch(const Event&);
				
			private:
				ConnectionInfo *head;
		};
	}
}

#endif

