#include <memory>

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
#include <glx/impl/extensions.hpp>

#include <X11/Xlib.h>
#include <GL/glx.h>

namespace 
{
	void checked_glXGetConfig(Display *xdisplay, XVisualInfo *visualinfo, int attrib, int &value)
	{
		int val;
		if(glXGetConfig(xdisplay, visualinfo, attrib, &val))
			throw wm::Exception("glXGetConfig failed");
		value = val;
	}

	struct LegacyPixelFormatBuilder : public wm::glx::PixelFormatBuilder
	{
		LegacyPixelFormatBuilder(Display* xdisplay, int screen)
			: xdisplay(xdisplay)
			, screen(screen)
		{
			long template_mask = VisualScreenMask;
			XVisualInfo vi_template;
			vi_template.screen = screen;
			
			int num;
			XVisualInfo *vi =
				XGetVisualInfo(xdisplay, template_mask, &vi_template, &num);
			if(!vi || num <= 0)
			{
				XFree(vi);
				throw wm::Exception("XGetVisualInfo failed");
			}
				
			visualInfos = vi;
			numVisualInfos = num;
			
		}
		
		~LegacyPixelFormatBuilder()
		{
			XFree(visualInfos);
		}

		Display* xdisplay;
		int screen;
		
		XVisualInfo *visualInfos;
		int numVisualInfos;

		virtual int numFormats() const { return numVisualInfos; }

		virtual bool filterFormat(int index) const
		{
			int use_gl, doublebuffer, rgba;
			
			XVisualInfo *vi = getVisualInfo(index);
			checked_glXGetConfig(xdisplay, vi, GLX_USE_GL, use_gl);
			checked_glXGetConfig(xdisplay, vi, GLX_DOUBLEBUFFER, doublebuffer);
			checked_glXGetConfig(xdisplay, vi, GLX_RGBA, rgba);
			
			return !use_gl || !doublebuffer || !rgba;			
		}
		
		virtual wm::PixelFormat::Descriptor makeDescriptor(int index) const
		{
			int red, green, blue, alpha;
			int depth, stencil;

			XVisualInfo *vi = getVisualInfo(index);
			checked_glXGetConfig(xdisplay, vi, GLX_RED_SIZE, red);
			checked_glXGetConfig(xdisplay, vi, GLX_GREEN_SIZE, green);
			checked_glXGetConfig(xdisplay, vi, GLX_BLUE_SIZE, blue);
			checked_glXGetConfig(xdisplay, vi, GLX_ALPHA_SIZE, alpha);

			checked_glXGetConfig(xdisplay, vi, GLX_DEPTH_SIZE, depth);
			checked_glXGetConfig(xdisplay, vi, GLX_STENCIL_SIZE, stencil);

			return wm::PixelFormat::Descriptor(red, green, blue, alpha, depth, stencil);
		}
		
		virtual void getVisual(int index, Visual *& visual, int &depth) const
		{
			XVisualInfo *vi = getVisualInfo(index);
			visual = vi->visual;
			depth = vi->depth;	
		};

		virtual XVisualInfo* getVisualInfo(int index) const
		{
			if(index < 0 || index >= numVisualInfos)
				throw wm::Exception("VisualInfo index out of bounds");
				
			return visualInfos + index;
		}
		
#ifdef GLX_VERSION_1_3
		virtual GLXFBConfig getFBConfig(int index) const { return 0; }
#endif		
	};

#ifdef GLX_VERSION_1_3
	void checkedGetFBConfigAttrib(const wm::glx::Extensions &extensions, Display *xdisplay, GLXFBConfig config, int attribute, int &value)
	{
		if(extensions.glXGetFBConfigAttrib(xdisplay, config, attribute, &value) != Success)
		{
			throw wm::Exception("Can't query framebuffer configuration attributes");
		}
	}

	struct GLX_1_3_PixelFormatBuilder : public wm::glx::PixelFormatBuilder
	{
		GLX_1_3_PixelFormatBuilder(const wm::glx::Extensions &extensions, Display *xdisplay, int screen)
			: extensions(extensions)
			, xdisplay(xdisplay)
			, screen(screen)
			, numConfigs(0)
			, fbconfigs(0)
		{
			int num;
			GLXFBConfig *configs = extensions.glXGetFBConfigs(xdisplay, screen, &num);
				
			if(!configs || num <= 0)
			{
				XFree(configs);
				throw wm::Exception("Can't query framebuffer configurations");
			}
			
			numConfigs = num;
			fbconfigs = configs;
			
		}
		
		~GLX_1_3_PixelFormatBuilder()
		{
			XFree(fbconfigs);	
		}
		
		const wm::glx::Extensions &extensions;
		Display *xdisplay;
		int screen;
		
		int numConfigs;
		GLXFBConfig *fbconfigs;
	
		virtual int numFormats() const { return numConfigs; }
		
		virtual bool filterFormat(int index) const
		{
			int render_type, drawable_type, doublebuffer;
							
			GLXFBConfig config = getFBConfig(index);							
			checkedGetFBConfigAttrib(extensions, xdisplay, config, GLX_RENDER_TYPE, render_type);
			checkedGetFBConfigAttrib(extensions, xdisplay, config, GLX_DRAWABLE_TYPE, drawable_type);
			checkedGetFBConfigAttrib(extensions, xdisplay, config, GLX_DOUBLEBUFFER, doublebuffer);
			
			return (!(render_type & GLX_RGBA_BIT) || !(drawable_type & GLX_WINDOW_BIT) || !doublebuffer);
		}
		
		virtual wm::PixelFormat::Descriptor makeDescriptor(int index) const 
		{
			int red, green, blue, alpha;
			int depth, stencil;
			
			GLXFBConfig config = getFBConfig(index);
			checkedGetFBConfigAttrib(extensions, xdisplay, config, GLX_RED_SIZE, red);
			checkedGetFBConfigAttrib(extensions, xdisplay, config, GLX_GREEN_SIZE, green);
			checkedGetFBConfigAttrib(extensions, xdisplay, config, GLX_BLUE_SIZE, blue);
			checkedGetFBConfigAttrib(extensions, xdisplay, config, GLX_ALPHA_SIZE, alpha);

			checkedGetFBConfigAttrib(extensions, xdisplay, config, GLX_DEPTH_SIZE, depth);
			checkedGetFBConfigAttrib(extensions, xdisplay, config, GLX_STENCIL_SIZE, stencil);
		
			return wm::PixelFormat::Descriptor(red, green, blue, alpha, depth, stencil);
		}
		
		virtual void getVisual(int index, Visual *& visual, int &depth) const
		{
			XVisualInfo *vi = extensions.glXGetVisualFromFBConfig(xdisplay, getFBConfig(index));
			if(!vi)
				throw wm::Exception("glXGetVisualFromFBConfig failed");
				
			visual = vi->visual;
			depth = vi->depth;
			
			XFree(vi);
		}

		virtual XVisualInfo* getVisualInfo(int index) const { return 0; }
		virtual GLXFBConfig getFBConfig(int index) const
		{
			if(index < 0 || index >= numConfigs)
				throw wm::Exception("FBConfig index out of bounds");
			return fbconfigs[index];
		}
	};
#endif
	
	const wm::glx::PixelFormatBuilder* makeBuilder(const wm::glx::Extensions &extensions, ::Display* xdisplay, int screen)
	{
#ifdef GLX_VERSION_1_3
		if(extensions.supported(1, 3))
			return new GLX_1_3_PixelFormatBuilder(extensions, xdisplay, screen);
#endif

		return new LegacyPixelFormatBuilder(xdisplay, screen);
	}	
}

namespace wm
{
	Configuration::Configuration(Display &display)
		: impl(new impl_t)
		, display_(&display)
	{
		std::auto_ptr<impl_t> impl_guard(impl); // deletes impl object in case of exception
	
		::Display *xdisplay = display.impl->display;
		
		int screen = DefaultScreen(xdisplay);		// TODO: make this work on different screens
		impl->extensions.init(xdisplay, screen);
		
		impl->builder.reset(makeBuilder(impl->extensions, xdisplay, screen));
	
		for(int index = 0; index < impl->builder->numFormats(); ++index)
		{
			if(impl->builder->filterFormat(index)) continue;

			PixelFormat::impl_t formatimpl;
			PixelFormat::Descriptor desc = impl->builder->makeDescriptor(index);
			
			formatimpl.visualinfo = impl->builder->getVisualInfo(index);
			impl->builder->getVisual(index, formatimpl.visual, formatimpl.depth);

#ifdef GLX_VERSION_1_3
			formatimpl.fbconfig = impl->builder->getFBConfig(index);
#endif
			
			impl->formatdata.push_back(formatimpl);
			impl->formats.push_back(PixelFormat(desc, *this, impl->formatdata.back()));
		}


		impl_guard.release();
	}

	Configuration::~Configuration()
	{
		delete impl;
	}

	int Configuration::numFormats() const { return impl->formats.size(); }
	const PixelFormat& Configuration::getFormat(int index) const { return impl->formats.at(index); }

	void PixelFormat::set(Window& window) const
	{
		::Display *xdisplay = window.display().impl->display;
	
		window.impl->visual = impl->visual;
		window.impl->depth = impl->depth;		
	}
}

