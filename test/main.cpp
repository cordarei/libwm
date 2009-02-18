#ifndef WIN32
#include <unistd.h>
#endif

#include <iostream>

#include <wm/display.hpp>
#include <wm/window.hpp>
#include <wm/pixelformat.hpp>
#include <wm/context.hpp>

#include <wm/connection.hpp>
#include <wm/eventhandler.hpp>

namespace test { void draw(); }

namespace
{
	void connectionTest(wm::Window &window)
	{
		struct Handler : public wm::EventHandler
		{
			explicit Handler(int id) : id(id) { };
		
			int id;
		
			virtual void expose(unsigned int x, unsigned int y, unsigned int w, unsigned int h)
			{
				std::cout 
					<< "id: " << id << "  "
					<< "expose: " << x << ", " << y << ", " << w << ", " << h << std::endl;
			}
		} handler1(1), handler2(2);
	
		wm::Connection connection1(window, handler1);
		wm::Connection connection2(window, handler2);

		std::cout << (connection1.connected()?"":"not ") << "connected" << std::endl;
		window.invoke_expose(1,2,3,4);

		connection1.disconnect();

		std::cout << (connection1.connected()?"":"not ") << "connected" << std::endl;
		window.invoke_expose(4,3,2,1);	
	}
}

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

	connectionTest(window);	
	window.invoke_expose(4,3,2,1);	

	makeCurrent(window, context);
	
	window.show();
	window.swap();

#ifdef WIN32
	Sleep(1000);
#else
	sleep(1);
#endif
	
	test::draw();
	window.swap();
	
#ifdef WIN32
	Sleep(1000);
#else
	sleep(1);
#endif

	return 0;
}

