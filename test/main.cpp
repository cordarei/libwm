#include <unistd.h>

#include <wm/display.hpp>
#include <wm/window.hpp>
#include <wm/pixelformat.hpp>
#include <wm/context.hpp>

namespace test { void draw(); }

int main(int argc, char *argv[])
{
	wm::Display display;
	wm::Window window(display, 0, 400, 300);
	wm::Context context(window, 1, 2);
	
	makeCurrent(window, context);
	
	window.show();
	window.swap();
	sleep(1);
	
	test::draw();
	window.swap();
	
	sleep(1);
}

