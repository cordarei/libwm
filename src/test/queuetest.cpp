#include <iostream>

#include <wm/event.hpp>
#include <wm/events.hpp>
#include <common/eventqueue.hpp>

namespace wm
{
	class Window;
}

int main(int, char*[])
{
	wm::common::EventQueue queue;
	wm::Window &window = *static_cast<wm::Window*>(0); // not dereferenced anywhere
	
	queue.push(new wm::ExposeEvent(window, 1, 2, 3, 4));
	queue.push(new wm::ButtonEvent(window, 1, 2, 3, false));
	
	boost::scoped_ptr<const wm::Event> event;
	while(queue.poll(event))
	{
		std::cout << "Event!" << std::endl;
	}

	queue.push(new wm::ButtonEvent(window, 1, 2, 3, false));

	while(queue.poll(event))
	{
		std::cout << "Event!" << std::endl;
	}

	queue.push(new wm::ButtonEvent(window, 1, 2, 3, false));
}

