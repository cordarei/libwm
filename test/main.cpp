#include <iostream>

#include <wm/display.hpp>
#include <wm/window.hpp>
#include <wm/pixelformat.hpp>
#include <wm/context.hpp>

#include <wm/connection.hpp>
#include <wm/eventhandler.hpp>
#include <wm/event.hpp>

namespace test { void draw(); }

#ifdef WIN32
#include <windows.h>
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR /* cmdLine */, int)
#else
int main(int argc, char *argv[])
#endif
{
	wm::Display display;
	wm::Window window(display, 0, 400, 300);
	wm::Context context(window, 1, 2);

	struct Handler : public wm::EventHandler
	{
		explicit Handler(wm::Window &window) : window(window) {}
	
		virtual void handle(const wm::ExposeEvent &event)
		{
			std::cout 
				<< "expose"
				<< ": " << event.x()
				<< ", " << event.y()
				<< ", " << event.width()
				<< ", " << event.height()
				<< std::endl;

			test::draw();
			window.swap();
		}
		
		wm::Window &window;
	} handler(window);

	wm::Connection connection(window, handler);

	makeCurrent(window, context);
	
	window.show();
	window.swap();

	for(int i = 0; i < 10; ++i) window.dispatch(true);
	
	return 0;
}

