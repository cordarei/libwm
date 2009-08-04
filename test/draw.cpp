#include <wm/opengl/opengl.hpp>

#include "draw.hpp"

namespace test
{
	void draw(unsigned int width, unsigned int height, bool flipped)
	{
		glViewport(0, 0, width, height);
	
		glClearColor(0.2f, 0.4f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glBegin(GL_TRIANGLES);
			glVertex2f(0.0f, flipped ? -1.0f : 1.0f);
			glVertex2f(-1.0f, flipped ? 1.0f : -1.0f);
			glVertex2f(1.0f, flipped ? 1.0f : -1.0f);
		glEnd();
	}
}

