#include <sstream>
#include <stdexcept>

#include <wm/opengl/opengl.hpp>

#include "draw.hpp"

namespace 
{
	bool checkGLError(const char *message, GLenum ignore = GL_NO_ERROR)
	{
		GLenum err;
		std::string msg;
		bool success = true, found = false;
		
		while((err = glGetError()) != GL_NO_ERROR)
		{
			if(err == ignore)
			{
				found = true;
				continue;
			}
			
			success = false;
			const GLubyte *ptr = gluErrorString(err);
			if(ptr)
			{
				msg += reinterpret_cast<const char*>(ptr);
				msg.push_back(' ');
			}
		}
		
		if(!success) throw std::runtime_error(message + msg);
		return found;
	}

	void parseVersion(const GLubyte *str, int &major, int &minor)
	{
		std::istringstream iss(reinterpret_cast<const char*>(str));
		iss.exceptions(std::ios::badbit | std::ios::failbit);
		int min, maj;
		iss >> maj;
		if(iss.get() != '.') throw std::runtime_error("Malformed version string");
		iss >> min;

		major = maj;
		minor = min;
	}

	void getGLVersion(int &major, int &minor)
	{
		const GLubyte *versionString = glGetString(GL_VERSION);
		parseVersion(versionString, major, minor);
		checkGLError("Can't query OpenGL version: ");
	}
}

namespace test
{	
	void checkGLversion(int &major, int &minor)
	{
#ifndef GL_MAJOR_VERSION
		const GLenum GL_MAJOR_VERSION = 0x821B,
			GL_MINOR_VERSION = 0x821C;
#endif
		
		GLint maj, min;
		glGetIntegerv(GL_MAJOR_VERSION, &maj);
		if(checkGLError("Can't query OpenGL major version", GL_INVALID_ENUM))
		{
			getGLVersion(major, minor);
			return;
		}
		
		glGetIntegerv(GL_MINOR_VERSION, &min);
		checkGLError("Can't query OpenGL minor version");

		major = maj; minor = min;
	}

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

