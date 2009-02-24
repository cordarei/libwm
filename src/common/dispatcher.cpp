#include <boost/ref.hpp>
#include <boost/bind.hpp>

#include <wm/event.hpp>
#include <wm/eventhandler.hpp>

#include <common/dispatcher.hpp>

namespace wm
{
	namespace common
	{
		void Dispatcher::connect(EventHandler &handler, ConnectionInfo &info)
		{
			{
				// TODO: Synchronization goes here
				if(info.connected) return;
			
				info.iterator = handlers.insert(handlers.begin(), &handler);
				info.connected = true;
			}
		}

		void Dispatcher::disconnect(ConnectionInfo &info)
		{
			{
				// TODO: Synchronization goes here
				if(!info.connected) return;		
			
				handlers.erase(info.iterator);
				info.connected = false;
			}
		}
	
		bool Dispatcher::isConnected(ConnectionInfo &info)
		{
			{
				// TODO: Synchronization goes here
				return info.connected;
			}
		}
		
	
		void Dispatcher::dispatch(
			const Event &event
			)
		{
			typedef std::list<wm::EventHandler*>::const_iterator iterator;
			using boost::bind;
			using boost::ref;
		
			for(iterator i = handlers.begin();
				i != handlers.end();
				++i)
			{
				wm::EventHandler& handler = **i;
				event.accept(handler);
			}

		}

	}
}

