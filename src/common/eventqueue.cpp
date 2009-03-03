#include <iostream>

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include <wm/event.hpp>

#include <common/eventqueue.hpp>

namespace wm
{
	namespace common
	{
		struct EventQueue::Node : boost::noncopyable
		{
			Node(const Event* event, Node *next = 0, Node *prev = 0)
				: event(event)
				, next(next)
				, prev(prev)
			{
			}
		
			boost::scoped_ptr<const Event> event;
			Node *next, *prev;
		};
	
		EventQueue::EventQueue()
			: head(0)
			, tail(0)
		{
		}
	
		EventQueue::~EventQueue()
		{
			Node *node = head;
			head = tail = 0;
		
			Node *next = 0;
			for(; node; node = next)
			{
				next = node->next;
				delete node;
			}
		}	
	
		void EventQueue::push(const Event *event)
		{
			// scoped_ptr takes care of cleanup in case of exceptions
			boost::scoped_ptr<const Event> ptr(event);
	
			if(!head) // queue is empty
			{
				if(tail) std::cerr << "Inconsistent EventQueue state, !head && tail" << std::endl;
				head = tail = new Node(0);
				ptr.swap(head->event); // change ownership
			} else
			{
				head = new Node(0, head);
				ptr.swap(head->event); // change ownership
				head->next->prev = head;
			}
		}
	
		bool EventQueue::poll(boost::scoped_ptr<const Event> &event)
		{
			if(!tail) // queue is empty
				return false;

			// remove node from queue
			boost::scoped_ptr<Node> node(tail);
			tail = node->prev;
			if(tail) tail->next = 0;
			if(head == node.get()) head = 0;

			// change ownership of Event object
			node->event.swap(event);

			return event.get() ? true : false; // should never be false
		}
	}
}

