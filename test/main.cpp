#ifndef WIN32
#include <unistd.h>
#endif

#include <wm/display.hpp>
#include <wm/window.hpp>
#include <wm/pixelformat.hpp>
#include <wm/context.hpp>

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
	
	makeCurrent(window, context);
	
	window.show();
	window.swap();
	//sleep(1);
	
	test::draw();
	window.swap();
	
	//sleep(1);
}

