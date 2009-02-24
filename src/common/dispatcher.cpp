#include <wm/event.hpp>
#include <wm/eventhandler.hpp>

#include <common/dispatcher.hpp>

namespace wm
{
	namespace common
	{
		struct ConnectionInfo
		{
			explicit ConnectionInfo(
				EventHandler &handler,
				ConnectionInfo *next = 0,
				ConnectionInfo *prev = 0)
				: handler(handler)
				, next(next)
				, prev(prev)
			{
			}
		
			EventHandler &handler;
			ConnectionInfo *next, *prev;
		};
	
		Dispatcher::Dispatcher()
			: head(0)
		{
		}
		
		Dispatcher::~Dispatcher()
		{
			ConnectionInfo *node = 0;
			
			{
				// TODO: Synchronization goes here
				node = head;
				head = 0;
			}
			
			ConnectionInfo *next = 0;
			for(; node; node = next)
			{
				next = node->next;
				delete node;
			}
		}
	
		ConnectionInfo* Dispatcher::connect(EventHandler &handler)
		{
			{
				// TODO: Synchronization goes here
				if(!head)  // empty list
				{
					head = new ConnectionInfo(handler);
				} else
				{
					head = new ConnectionInfo(handler, head);
					if(head->next) head->next->prev = head;
				}
				
				return head;
			}
		}

		void Dispatcher::disconnect(ConnectionInfo *info)
		{
			if(info)
			{
				// TODO: Synchronization goes here
				if(info->prev) info->prev->next = info->next;
				if(info->next) info->next->prev = info->prev;
				if(head == info) head = info->next; // first node removed
				delete info;
			}
		}

		void Dispatcher::dispatch(
			const Event &event
			)
		{
			// TODO: Synchronize this!
			for(ConnectionInfo *node = head;
				node;
				node = node->next)
			{
				event.accept(node->handler);
			}
		}
	}
}

