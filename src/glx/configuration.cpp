#include <wm/exception.hpp>
#include <wm/display.hpp>
#include <wm/window.hpp>
#include <wm/configuration.hpp>
#include <wm/pixelformat.hpp>
#include <wm/surface.hpp>

#include <xlib/impl/display_impl.hpp>
#include <xlib/impl/window_impl.hpp>
#include <glx/impl/configuration_impl.hpp>
#include <glx/impl/pixelformat_impl.hpp>

#include <X11/Xlib.h>
#include <GL/glx.h>

namespace 
{
	void getVersion(Display* xdisplay, int &versionMajor, int &versionMinor)
	{
		
	}
	
	void checkedGetFBConfigAttrib(Display *xdisplay, GLXFBConfig config, int attribute, int &value)
	{
		if(glXGetFBConfigAttrib(xdisplay, config, attribute, &value) != Success)
		{
			throw wm::Exception("Can't query framebuffer configuration attributes");
		}
	}
}

namespace wm
{
	Configuration::Configuration(Window &window)
		: impl(new impl_t(window.display()))
	{
		::Display *xdisplay = window.display().impl->display;
		getVersion(xdisplay, impl->versionMajor, impl->versionMinor);
		
		int numConfigs;
		GLXFBConfig *fbconfigs =
			glXGetFBConfigs(
				xdisplay,
				window.impl->screen,
				&numConfigs);
				
		if(!fbconfigs || numConfigs <= 0)
		{
			throw wm::Exception("Can't query framebuffer configurations");
		}
		
		for(GLXFBConfig* config = fbconfigs; config != fbconfigs + numConfigs; ++config)
		{
			int render_type, drawable_type;
							
			checkedGetFBConfigAttrib(xdisplay, *config, GLX_RENDER_TYPE, render_type);
			checkedGetFBConfigAttrib(xdisplay, *config, GLX_DRAWABLE_TYPE, drawable_type);
			
			if(!(render_type & GLX_RGBA_BIT) || !(drawable_type & GLX_WINDOW_BIT))
				continue;
				
			impl->formatdata.push_back(PixelFormat::impl_t());
			PixelFormat::impl_t &format = impl->formatdata.back();
			
			format.fbconfig = *config;

			checkedGetFBConfigAttrib(xdisplay, *config, GLX_RED_SIZE, format.red);
			checkedGetFBConfigAttrib(xdisplay, *config, GLX_GREEN_SIZE, format.green);
			checkedGetFBConfigAttrib(xdisplay, *config, GLX_BLUE_SIZE, format.blue);
			checkedGetFBConfigAttrib(xdisplay, *config, GLX_ALPHA_SIZE, format.alpha);

			checkedGetFBConfigAttrib(xdisplay, *config, GLX_DEPTH_SIZE, format.depth);
			checkedGetFBConfigAttrib(xdisplay, *config, GLX_STENCIL_SIZE, format.stencil);
		}
	}

	Configuration::~Configuration()
	{
		delete impl;
	}

	int Configuration::numFormats() const { return impl->formatdata.size(); }
	PixelFormat Configuration::getFormat(int index) const { return PixelFormat(*this, &impl->formatdata.at(index)); }

}

