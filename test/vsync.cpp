#include <sstream>
#include <cmath>

#include <wm/wm.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

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
	
	wm::Window window(display, 400, 300, choose(config));
	wm::Surface surface(window);
	wm::Context context(window.pixelformat());
	
	wm::CurrentContext current(context, surface);
	
	struct EventHandler : public wm::EventHandler
	{
		EventHandler(wm::CurrentContext &current)
			: current(current)
			, vsync(true)
			, quit(false)
		{
			current.swapInterval(1);
		}
	
		void handle(const wm::CloseEvent&)
		{
			quit = true;
		}
		
		void handle(const wm::KeyEvent &event)
		{
			if(event.symbol() == wm::keyboard::F1 && event.state())
			{
				vsync = !vsync;
				current.swapInterval(vsync ? 1 : 0);
			}
			
			if(event.symbol() == wm::keyboard::ESCAPE) quit = true;
		}
		
		wm::CurrentContext &current;
		bool vsync;
		bool quit;
	} handler(current);
	
	wm::Connection connection(window, handler);
	window.show();
	
	int framecount = 0;
	boost::posix_time::ptime timer_start = boost::posix_time::microsec_clock::local_time();
	boost::posix_time::ptime frametimer = timer_start;
	
	while(!handler.quit)
	{
		boost::posix_time::ptime timer = boost::posix_time::microsec_clock::local_time();
		float elapsed_seconds = (timer - timer_start).total_milliseconds() / 1000.0f;

		draw(elapsed_seconds);
		surface.swap();

		if(++framecount >= 50)
		{
			float framesecs = (timer - frametimer).total_milliseconds() / 1000.0f;
			float fps = framecount / framesecs;
			framecount = 0;
			frametimer = timer;
			
			std::ostringstream oss;
			oss << "FPS: " << fps << " vsync " << (handler.vsync ? "enabled" : "disabled");
			window.setTitle(oss.str().c_str());
		}
		
		display.poll();
		window.dispatch();
	}

	return 0;
}

