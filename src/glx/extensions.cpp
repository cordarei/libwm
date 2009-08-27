#include <iostream>
#include <sstream>
#include <string>
#include <iterator>

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
	
	void buildStringSet(const char *delimited, std::set<std::string> &set)
	{
		std::istringstream iss(delimited);
		typedef std::istream_iterator<std::string> iter;
		set.clear();
		set.insert(iter(iss), iter());
	}
	
	void initExtensionsSet(::Display* xdisplay, int screen, std::set<std::string> &set)
	{
		const char *extstring = glXQueryExtensionsString(xdisplay, screen);
		buildStringSet(extstring, set);
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

	void initGLX_1_4(wm::glx::Extensions &extensions)
	{
		dlsym_wrapper(extensions.library, "glXGetProcAddress", extensions.glXGetProcAddress);
	}
	
	void initARB_get_proc_address(wm::glx::Extensions& extensions)
	{
		dlsym_wrapper(extensions.library, "glXGetProcAddressARB", extensions.glXGetProcAddressARB);
		extensions.ARB_get_proc_address = true;
	}

#ifdef GLX_VERSION_1_3	
	void initARB_create_context(wm::glx::Extensions& extensions)
	{
		extensions.getProcAddress(
			reinterpret_cast<const GLubyte*>("glXCreateContextAttribsARB"),
			extensions.glXCreateContextAttribsARB);
		extensions.ARB_create_context = true;
		extensions.ARB_create_context_profile = extensions.supported("GLX_ARB_create_context_profile");
	}
#endif

	void initARB_multisample(wm::glx::Extensions& extensions)
	{
		extensions.ARB_multisample = extensions.supported("GLX_ARB_multisample");
	}

	void initARB_framebuffer_sRGB(wm::glx::Extensions& extensions)
	{
		extensions.ARB_framebuffer_sRGB = extensions.supported("GLX_ARB_framebuffer_sRGB");
	}	

	void initEXT_framebuffer_sRGB(wm::glx::Extensions& extensions)
	{
		extensions.EXT_framebuffer_sRGB = extensions.supported("GLX_EXT_framebuffer_sRGB");
	}	

	void initARB_fbconfig_float(wm::glx::Extensions& extensions)
	{
		extensions.ARB_fbconfig_float = extensions.supported("GLX_ARB_fbconfig_float");
	}	

	void initEXT_packed_float(wm::glx::Extensions& extensions)
	{
		extensions.EXT_packed_float = extensions.supported("GLX_EXT_packed_float");
	}
	
	void initEXT_visual_rating(wm::glx::Extensions& extensions)
	{
		extensions.EXT_visual_rating = extensions.supported("GLX_EXT_visual_rating");
	}	

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
				initExtensionsSet(xdisplay, screen, extensions);
				
#ifdef GLX_VERSION_1_3
				if(supported(1, 3)) initGLX_1_3(*this);
#endif

				if(supported(1, 4)) initGLX_1_4(*this);
				if(supported("GLX_ARB_get_proc_address")) initARB_get_proc_address(*this);
				
#ifdef GLX_VERSION_1_3
				if(supported("GLX_ARB_create_context")) initARB_create_context(*this);
#endif

				initARB_multisample(*this);
				initARB_framebuffer_sRGB(*this);
				initEXT_framebuffer_sRGB(*this);
				initARB_fbconfig_float(*this);
				initEXT_packed_float(*this);
				initEXT_visual_rating(*this);
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

		bool Extensions::supported(const std::string &ext) const
		{
			typedef std::set<std::string>::const_iterator set_iterator;
			set_iterator iter = extensions.find(ext);
			return iter != extensions.end();
		}		

		Extensions::GLXextFuncPtr Extensions::getProcAddress(const GLubyte *name) const
		{
			GLXextFuncPtr (*getProcAddressFunc)(const GLubyte*)
				= (glXGetProcAddress ? glXGetProcAddress : glXGetProcAddressARB);
			if(!getProcAddressFunc)
				throw wm::Exception("Can't load GLX extension function: GLX 1.4 or GLX_get_proc_address not supported");
			GLXextFuncPtr ptr = getProcAddressFunc(name);
			
			if(!ptr)
				throw wm::Exception("Can't load GLX extension function " +
					std::string(reinterpret_cast<const char*>(name)));
					
			return ptr;
		}
	}
}

