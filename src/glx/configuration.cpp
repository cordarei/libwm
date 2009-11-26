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

namespace wm
{
	namespace glx
	{
		struct PixelFormatBuilder
		{
			virtual ~PixelFormatBuilder() {};
			virtual int numFormats() const = 0;
			virtual bool filterFormat(int index) const = 0;
			virtual wm::PixelFormat::Descriptor makeDescriptor(int index) const = 0;
			virtual void getVisual(int index, Visual *& visual, int &depth) const = 0;
		
			virtual XVisualInfo* getVisualInfo(int index) const = 0;
#ifdef GLX_VERSION_1_3
			virtual GLXFBConfig getFBConfig(int index) const = 0;
			virtual int getRenderType(int index) const = 0;
#endif
		};
	}
}

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
		LegacyPixelFormatBuilder(const wm::glx::Extensions& extensions, Display* xdisplay, int screen)
			: extensions(extensions)
			, xdisplay(xdisplay)
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
		
		const wm::glx::Extensions extensions;
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
			int samples = 0, buffers = 0;
			int srgb = 0;

			XVisualInfo *vi = getVisualInfo(index);
			checked_glXGetConfig(xdisplay, vi, GLX_RED_SIZE, red);
			checked_glXGetConfig(xdisplay, vi, GLX_GREEN_SIZE, green);
			checked_glXGetConfig(xdisplay, vi, GLX_BLUE_SIZE, blue);
			checked_glXGetConfig(xdisplay, vi, GLX_ALPHA_SIZE, alpha);

			checked_glXGetConfig(xdisplay, vi, GLX_DEPTH_SIZE, depth);
			checked_glXGetConfig(xdisplay, vi, GLX_STENCIL_SIZE, stencil);
			
			if(extensions.ARB_multisample)
			{
				checked_glXGetConfig(xdisplay, vi, GLX_SAMPLES_ARB, samples);
				checked_glXGetConfig(xdisplay, vi, GLX_SAMPLE_BUFFERS_ARB, buffers);
			}
			
			if(extensions.ARB_framebuffer_sRGB)
			{
				checked_glXGetConfig(xdisplay, vi, GLX_FRAMEBUFFER_SRGB_CAPABLE_ARB, srgb);
			} else if(extensions.EXT_framebuffer_sRGB)
			{
				checked_glXGetConfig(xdisplay, vi, GLX_FRAMEBUFFER_SRGB_CAPABLE_EXT, srgb);
			}
			
			bool slow = false;
			if(extensions.EXT_visual_rating)
			{
				int caveat;
				checked_glXGetConfig(xdisplay, vi, GLX_VISUAL_CAVEAT_EXT, caveat);
				slow = (caveat == GLX_SLOW_VISUAL_EXT);
			}

			return wm::PixelFormat::Descriptor(
				red, green, blue, alpha,
				depth, stencil,
				samples, buffers,
				srgb != 0,
				wm::PixelFormat::INTEGER,
				slow);
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
		virtual GLXFBConfig getFBConfig(int) const { return 0; }

		virtual int getRenderType(int index) const
		{
			int rgba;

			XVisualInfo *vi = getVisualInfo(index);
			checked_glXGetConfig(xdisplay, vi, GLX_RGBA, rgba);

			return rgba ? GLX_RGBA_BIT : GLX_COLOR_INDEX_BIT;
		}
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

			int render_types = GLX_RGBA_BIT
				| (extensions.ARB_fbconfig_float ? GLX_RGBA_FLOAT_BIT_ARB : 0)
				| (extensions.EXT_fbconfig_packed_float ? GLX_RGBA_UNSIGNED_FLOAT_BIT_EXT : 0);
			
			return (!(render_type & render_types) || !(drawable_type & GLX_WINDOW_BIT) || !doublebuffer);
		}
		
		virtual wm::PixelFormat::Descriptor makeDescriptor(int index) const 
		{
			int red, green, blue, alpha;
			int depth, stencil;
			int samples, buffers;
			int srgb = 0;
			int caveat;
			
			GLXFBConfig config = getFBConfig(index);
			checkedGetFBConfigAttrib(extensions, xdisplay, config, GLX_RED_SIZE, red);
			checkedGetFBConfigAttrib(extensions, xdisplay, config, GLX_GREEN_SIZE, green);
			checkedGetFBConfigAttrib(extensions, xdisplay, config, GLX_BLUE_SIZE, blue);
			checkedGetFBConfigAttrib(extensions, xdisplay, config, GLX_ALPHA_SIZE, alpha);

			checkedGetFBConfigAttrib(extensions, xdisplay, config, GLX_DEPTH_SIZE, depth);
			checkedGetFBConfigAttrib(extensions, xdisplay, config, GLX_STENCIL_SIZE, stencil);

			checkedGetFBConfigAttrib(extensions, xdisplay, config, GLX_SAMPLES, samples);
			checkedGetFBConfigAttrib(extensions, xdisplay, config, GLX_SAMPLE_BUFFERS, buffers);

			if(extensions.ARB_framebuffer_sRGB)
			{
				checkedGetFBConfigAttrib(extensions, xdisplay, config, GLX_FRAMEBUFFER_SRGB_CAPABLE_ARB, srgb);
			} else if(extensions.EXT_framebuffer_sRGB)
			{
				checkedGetFBConfigAttrib(extensions, xdisplay, config, GLX_FRAMEBUFFER_SRGB_CAPABLE_EXT, srgb);
			}
			
			checkedGetFBConfigAttrib(extensions, xdisplay, config, GLX_CONFIG_CAVEAT, caveat);
		
			return wm::PixelFormat::Descriptor(
				red, green, blue, alpha,
				depth, stencil,
				samples, buffers,
				srgb != 0,
				wm::PixelFormat::INTEGER,
				caveat == GLX_SLOW_CONFIG);
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

		virtual XVisualInfo* getVisualInfo(int) const { return 0; }
		virtual GLXFBConfig getFBConfig(int index) const
		{
			if(index < 0 || index >= numConfigs)
				throw wm::Exception("FBConfig index out of bounds");
			return fbconfigs[index];
		}
		
		virtual int getRenderType(int index) const
		{
			int render_type;		

			GLXFBConfig config = getFBConfig(index);
			checkedGetFBConfigAttrib(extensions, xdisplay, config, GLX_RENDER_TYPE, render_type);
			
			return render_type;
		}
	};
#endif
	
	const wm::glx::PixelFormatBuilder* makeBuilder(const wm::glx::Extensions &extensions, ::Display* xdisplay, int screen)
	{
#ifdef GLX_VERSION_1_3
		if(extensions.supported(1, 3))
			return new GLX_1_3_PixelFormatBuilder(extensions, xdisplay, screen);
#endif

		return new LegacyPixelFormatBuilder(extensions, xdisplay, screen);
	}	
}

namespace wm
{
	Configuration::impl_t::~impl_t()	// boost::scoped_ptr dtor needs definition of PixelFormatBuilder
	{
	}

	Configuration::Configuration(Display &display)
		: impl(new impl_t)
		, display_(&display)
	{
		std::auto_ptr<impl_t> impl_guard(impl); // deletes impl object in case of exception
	
		::Display *xdisplay = display.impl->display;
		int screen = display.impl->screen;
		
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

#ifdef GLX_VERSION_1_3
			int render_type = impl->builder->getRenderType(index);
			if(render_type & GLX_RGBA_BIT)
#endif
			{
				impl->formats.push_back(PixelFormat(desc, *this, impl->formatdata.back()));
			}
			
#ifdef GLX_VERSION_1_3
			if(impl->extensions.ARB_fbconfig_float && render_type & GLX_RGBA_FLOAT_BIT_ARB)
			{
				PixelFormat::Descriptor d2(desc);
				d2.type = PixelFormat::FLOAT;
				impl->formats.push_back(PixelFormat(d2, *this, impl->formatdata.back()));
			}
			
			if(impl->extensions.EXT_fbconfig_packed_float && render_type & GLX_RGBA_UNSIGNED_FLOAT_BIT_EXT)
			{
				PixelFormat::Descriptor d2(desc);
				d2.type = PixelFormat::UNSIGNED_FLOAT;
				impl->formats.push_back(PixelFormat(d2, *this, impl->formatdata.back()));
			}
#endif
		}


		impl_guard.release();
	}

	Configuration::~Configuration()
	{
		delete impl;
	}

	int Configuration::numFormats() const { return impl->formats.size(); }
	const PixelFormat& Configuration::getFormat(int index) const { return impl->formats.at(index); }

	void* Configuration::getProcAddress(const char *name)
	{
		glx::Extensions::GLXextFuncPtr ptr = 
			impl->extensions.getProcAddress(reinterpret_cast<const GLubyte*>(name));
		return *reinterpret_cast<void**>(&ptr);
	}

	void PixelFormat::set(Window& window) const
	{
		window.impl->visual = impl->visual;
		window.impl->depth = impl->depth;		
	}
}

