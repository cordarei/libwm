#ifndef COMMON_EVENTQUEUE_HPP
#define COMMON_EVENTQUEUE_HPP

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

namespace wm
{
	class Event;

	namespace common
	{
		class EventQueue : boost::noncopyable
		{
			public:
				EventQueue();
				~EventQueue();

				void push(const Event *event);
				bool poll(boost::scoped_ptr<const Event> &event);

			private:
				struct Node;
				Node *head, *tail;
		};
	}
}

#endif

