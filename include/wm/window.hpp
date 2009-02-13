#ifndef WINDOW_HPP
#define WINDOW_HPP

namespace wm
{
	class Display;
	class PixelFormat;

	class Window
	{
		public:
			Window(
				Display& display,
				int screen,
				unsigned int width,
				unsigned int height,
				const PixelFormat& format);
				
			void show();
			void hide();
			
		private:
	};
}

#endif

