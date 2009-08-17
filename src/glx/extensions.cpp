#include <iostream>
#include <sstream>
#include <string>

#include <dlfcn.h>

#include <wm/exception.hpp>

#include <glx/impl/extensions.hpp>

namespace
{
	template <typename T>
	void dlsym_wrapper(void *library, const char *funcname, T *& funcptr)
	{
		void *ptr = dlsym(library, funcname);
		if(!ptr)
		{
			std::string error = dlerror();
			throw wm::Exception("Can't load symbol " + (funcname + (": " + error) ) );
		}
		
		funcptr = *reinterpret_cast<T**>(&ptr);
	}
	
	void parseVersionString(const char *str, int& versionMajor, int& versionMinor)
	{
		std::istringstream iss(str);
		iss.exceptions(std::ios::failbit | std::ios::badbit);
		int major, minor;
		iss >> major;
		if(iss.get() != '.') throw wm::Exception("malformed version string");
		iss >> minor;
		
		versionMajor = major;
		versionMinor = minor;
	}
	
	bool versionCmp(int aMajor, int aMinor, int bMajor, int bMinor)	// a < b
	{
		return aMajor < bMajor || (aMajor == bMajor && aMinor < bMinor);
	}
	
	void versionSelect(int aMajor, int aMinor, int bMajor, int bMinor, int &major, int &minor, bool second)
	{
		major = second ? bMajor : aMajor;
		minor = second ? bMinor : aMinor;
	}
	
	void queryGLXVersion(Display* xdisplay, int screen, int &versionMajor, int &versionMinor)
	{
		int major, minor;
		if(!glXQueryVersion(xdisplay, &major, &minor))
		{
			throw wm::Exception("glXQueryVersion failed");
		}

		const char *serverVersionStr = glXQueryServerString(xdisplay, screen, GLX_VERSION);
		if(!serverVersionStr) throw wm::Exception("glXQueryServerString failed");

		int serverMajor, serverMinor;
		parseVersionString(serverVersionStr, serverMajor, serverMinor);

		const char *clientVersionStr = glXGetClientString(xdisplay, GLX_VERSION);
		if(!clientVersionStr) throw wm::Exception("glXGetClientString failed");

		int clientMajor, clientMinor;
		parseVersionString(clientVersionStr, clientMajor, clientMinor);

		int ma, mi;
		versionSelect(clientMajor, clientMinor, serverMajor, serverMinor, ma, mi,
			versionCmp(serverMajor, serverMinor, clientMajor, clientMinor)); // min(client, server)
		versionSelect(major, minor, ma, mi, ma, mi, versionCmp(major, minor, ma, mi)); // max(version, min(client, server))
		
		versionMajor = ma;
		versionMinor = mi;
	}

#ifdef GLX_VERSION_1_3
	void initGLX_1_3(wm::glx::Extensions &extensions)
	{
		dlsym_wrapper(extensions.library, "glXGetFBConfigs", extensions.glXGetFBConfigs);
		dlsym_wrapper(extensions.library, "glXGetFBConfigAttrib", extensions.glXGetFBConfigAttrib);
		dlsym_wrapper(extensions.library, "glXGetVisualFromFBConfig", extensions.glXGetVisualFromFBConfig);

		dlsym_wrapper(extensions.library, "glXCreateWindow", extensions.glXCreateWindow);
		dlsym_wrapper(extensions.library, "glXDestroyWindow", extensions.glXDestroyWindow);
		dlsym_wrapper(extensions.library, "glXQueryDrawable", extensions.glXQueryDrawable);

		dlsym_wrapper(extensions.library, "glXCreateNewContext", extensions.glXCreateNewContext);
		dlsym_wrapper(extensions.library, "glXMakeContextCurrent", extensions.glXMakeContextCurrent);
	}
#endif

}

namespace wm
{
	namespace glx
	{
		Extensions::Extensions()
			: library(0)
		{
		}

		Extensions::~Extensions()
		{
			if(library)
			{
				if(dlclose(library))
				{
					const char *error = dlerror();
					std::cerr << "dlclose failed: " << (error ? error : "") << std::endl;
				}
			}
		}

		void Extensions::init(::Display *xdisplay, int screen)
		{
			if(library)
				throw wm::Exception("Extensions already initialized");
	
			library = dlopen(0, RTLD_NOW);
			if(!library)
			{
				std::string error = dlerror();
				throw wm::Exception("dlopen failed: " + error);
			}
			
			try
			{
				queryGLXVersion(xdisplay, screen, versionMajor, versionMinor);
				
#ifdef GLX_VERSION_1_3
				if(supported(1, 3)) initGLX_1_3(*this);
#endif
				
			} catch(...)
			{
				if(dlclose(library))
				{
					const char *error = dlerror();
					std::cerr << "dlclose failed: " << (error ? error : "") << std::endl;
				}
				
				throw;
			}
		}
		
		bool Extensions::supported(int major, int minor) const
		{
			bool val = !versionCmp(versionMajor, versionMinor, major, minor);
			return val;
		}
	}
}

