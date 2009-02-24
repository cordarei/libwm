#ifndef COMMON_DISPATCHER_HPP
#define COMMON_DISPATCHER_HPP

#include <list>

#include <boost/noncopyable.hpp>

namespace wm
{
	class Event;
	class EventHandler;

	namespace common
	{
		class Dispatcher : boost::noncopyable
		{
			public:
				class ConnectionInfo : boost::noncopyable
				{
						public:
							ConnectionInfo() : connected(false) {}
						private:
							bool connected;
							std::list<EventHandler*>::iterator iterator;
				
							friend class Dispatcher;
				};			
			
				void connect(EventHandler&, ConnectionInfo&);
				void disconnect(ConnectionInfo&);
				bool isConnected(ConnectionInfo&);
				
				void dispatch(const Event&);
				
			private:
				std::list<EventHandler*> handlers;
		};
	}
}

#endif

