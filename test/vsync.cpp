#include <sstream>
#include <cmath>

#include <wm/wm.hpp>
// #include <boost/date_time/posix_time/posix_time_types.hpp>

#include <wm/opengl/opengl.hpp>

namespace
{
	void draw(float timer)
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef(std::fmod(timer, 2.0f) * 360.0f, 0.0f, 0.0f, 1.0f);

		glBegin(GL_TRIANGLES);
		glVertex2f(0.0, -0.5);
		glVertex2f(-0.5, 0.5);
		glVertex2f(0.5, 0.5);
		glEnd();
	}
}

int wm_main(int, char*[])
{
	wm::Display display;
	wm::Configuration config(display);

    wm::SimpleQueue queue;
	wm::Window window(display, 400, 300, choose(config), queue);
	wm::Surface surface(window);
	wm::Context context(window.pixelformat());

	wm::CurrentContext current(context, surface);

	window.show();

	// boost::posix_time::ptime timer_start = boost::posix_time::microsec_clock::local_time();
	// boost::posix_time::ptime frametimer = timer_start;

    bool vsync = config.caps().swapControl;
    if(vsync) current.swapInterval(1);

	int framecount = 0;
    bool quit_flag = false;
	while(!quit_flag)
	{
		// boost::posix_time::ptime timer = boost::posix_time::microsec_clock::local_time();
		// float elapsed_seconds = (timer - timer_start).total_milliseconds() / 1000.0f;
        float elapsed_seconds = 0;

		draw(elapsed_seconds);
		surface.swap();

		if(++framecount >= 50)
		{
			// float framesecs = (timer - frametimer).total_milliseconds() / 1000.0f;
			// float fps = framecount / framesecs;
			// framecount = 0;
			// frametimer = timer;
            float fps = 0;

			std::ostringstream oss;
			oss << "FPS: " << fps << " vsync ";
			if(config.caps().swapControl) oss << (vsync ? "enabled" : "disabled");
			else oss << "unsupported";
			window.setTitle(oss.str().c_str());
		}

		display.poll();

        while(!queue.empty())
        {
            wm::Event event = queue.pop();

            if(event.type == wm::BUTTON_DOWN)
                quit_flag = true;
        }
	}

	return 0;
}

