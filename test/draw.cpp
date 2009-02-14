#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

namespace test
{
	void draw()
	{
		glClearColor(0.2f, 0.4f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glBegin(GL_TRIANGLES);
			glVertex2f(0.0f, 1.0f);
			glVertex2f(-1.0f, -1.0f);
			glVertex2f(1.0f, -1.0f);
		glEnd();
	}
}

