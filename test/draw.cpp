#include <GL/gl.h>

namespace test
{
	void draw()
	{
		glClearColor(0.2, 0.4, 0.7, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glBegin(GL_TRIANGLES);
			glVertex2f(0.0, 1.0);
			glVertex2f(-1.0, -1.0);
			glVertex2f(1.0, -1.0);
		glEnd();
	}
}

