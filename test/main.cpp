#include <unistd.h>

#include <wm/display.hpp>
#include <wm/window.hpp>
#include <wm/pixelformat.hpp>
#include <wm/context.hpp>

int main(int argc, char *argv[])
{
	wm::Display display;
	wm::Window window(display, 0, 400, 300);
	wm::Context ctx(window, 1, 2);
	
	window.show();
	
	sleep(1);
}

