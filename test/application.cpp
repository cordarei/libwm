#include <wm/wm.hpp>

#include <cmath>

#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

#include <wm/opengl/opengl.hpp>

class Application
{
	public:
		Application(const wm::PixelFormat::Descriptor &desc)
			: display(0)
			, config(display)
			, format(choose(config, desc))
			, win(*this, display, format)
			, model(Model())
			, quit(false)
		{
		}
	
		void run()
		{
			win.window.show();
			
			namespace pt = boost::posix_time;
			const pt::time_duration interval = pt::seconds(1) / 60,
				update_interval = pt::seconds(1) / 120;
			const float update_dt = 1.0 / 120.0f;
			pt::ptime next_redraw = pt::microsec_clock::local_time(),
				next_update = next_redraw;
			while(!quit)
			{
				pt::ptime timer = pt::microsec_clock::local_time();
				if(timer >= next_update)
				{
					for(pt::time_iterator iter(next_update, update_interval);
						iter < timer;
						++iter)
					{
						next_update = *iter + update_interval;
						model.update(update_dt);
					}					
				} else if(timer >= next_redraw)
				{
					win.draw();
					next_redraw = timer + interval;
				} else
				{
					display.dispatch(false);
					win.window.dispatch(false);

#undef min	// Damn windows.h macros
#undef max
					boost::this_thread::sleep(
						std::max(
							std::min(next_redraw, next_update) - pt::microsec_clock::local_time(),
							pt::time_duration(pt::milliseconds(1))							
						));
				}
			}
		}

	private:
		wm::Display display;
		wm::Configuration config;
		const wm::PixelFormat &format;
		
		struct EventHandler : public wm::EventHandler
		{
			explicit EventHandler(Application &app) : app(app) { }
			Application &app;
			
			virtual void handle(const wm::ExposeEvent& event)
			{
//				app.win.draw();
			}
			
			virtual void handle(const wm::CloseEvent&)
			{
				app.quit = true;
			}
			
			virtual void handle(const wm::KeyEvent& event)
			{
				if(event.state() && event.symbol() == wm::keyboard::ESCAPE)
					app.quit = true;
			}
		};

		struct AppWindow
		{
			AppWindow(Application &app, wm::Display &display, const wm::PixelFormat& format)
				: app(app)
				, window(display, 0, 400, 300, format)
				, context(format)
				, surface(window)
				, handler(app)
				, connection(window, handler, false)
			{
				connection.connect();
			}
			
			void draw()
			{
				unsigned int width, height;
				window.getSize(width, height);
			
				makeCurrent(context, surface, surface);
				
				glViewport(0, 0, width, height);
				
				glClearColor(0.2f, 0.4f, 0.9f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);
				
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				
				glRotatef(std::fmod(app.model.gametime * 360.0f, 360.0f), 0.0f, 0.0f, 1.0f);
				glBegin(GL_TRIANGLES);
				glVertex2f(0.0f, 0.5f);
				glVertex2f(-0.5f, -0.5f);
				glVertex2f(0.5f, -0.5f);
				glEnd();
				
				surface.swap();
			}

			Application &app;
			wm::Window window;
			wm::Context context;
			wm::Surface surface;
			EventHandler handler;
			wm::Connection connection;
		} win;
		
		struct Model
		{
			Model() : gametime(0) { }
			void update(float dt) { gametime += dt; }
			float gametime;
		} model;
		
		bool quit;
};

#include <iostream>

int wm_main(int, char*[])
{
	try
	{
		Application app(wm::PixelFormat::Descriptor(0, 0, 0, 0, 0, 0));
		app.run();
	} catch(wm::Exception &e)
	{
		std::cerr << "wm::Exception: " << e.what() << std::endl;
		return -1;
	}
	
	return 0;
}

