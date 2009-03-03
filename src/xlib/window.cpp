#include <algorithm>
#include <list>
#include <vector>

#include <X11/Xlib.h>
#include <GL/glx.h>

#include <wm/exception.hpp>
#include <wm/pixelformat.hpp>
#include <wm/display.hpp>
#include <wm/window.hpp>
#include <wm/event.hpp>

#include "impl/display_impl.hpp"
#include "impl/window_impl.hpp"
#include "impl/eventfactory.hpp"

namespace
{
	XVisualInfo *chooseVisual(Display *display, int screen, wm::PixelFormat format)
	{
		int attribs[] = {
			GLX_RGBA,
			GLX_DOUBLEBUFFER,
			GLX_RED_SIZE, format.red,
			GLX_BLUE_SIZE, format.blue,
			GLX_GREEN_SIZE, format.green,
			GLX_ALPHA_SIZE, format.alpha,
			GLX_DEPTH_SIZE, format.depth,
			GLX_STENCIL_SIZE, format.stencil };
			
		size_t size = sizeof(attribs) / sizeof(*attribs);
		std::vector<int> vec(size+1);
		std::copy(attribs + 0, attribs + size, vec.begin());
		vec.back() = None;
		
		XVisualInfo *info = glXChooseVisual(display, screen, &vec[0]);
		if(!info)
			throw wm::Exception("Can't select Visual");
			
		return info;
	}
}

namespace wm
{
	Window::Window(
		Display& display,
		int screen,
		unsigned int width,
		unsigned int height,
		const PixelFormat& format)
		: impl(new impl_t)
		, display_(display)
	{
		impl->event_mask = 
			StructureNotifyMask |
			ExposureMask |
//			PointerMotionMask |
			ButtonPressMask |
			ButtonReleaseMask |
			KeyPressMask |
			KeyReleaseMask |
			EnterWindowMask |
			LeaveWindowMask |
			FocusChangeMask |
			ResizeRedirectMask
			;
		
		impl->visualinfo = chooseVisual(
			display.impl->display,
			screen,
			format);
			
		XSetWindowAttributes attrib;
		attrib.colormap = XCreateColormap(
			display.impl->display,
			RootWindow(display.impl->display, screen),
			impl->visualinfo->visual,
			AllocNone);
			
		attrib.event_mask = impl->event_mask;
			
		impl->window = XCreateWindow(
			display.impl->display,
			RootWindow(display.impl->display, screen),
			0, 0,
			width, height,
			0,
			impl->visualinfo->depth,
			InputOutput,
			impl->visualinfo->visual,
			CWColormap | CWEventMask,
			&attrib
			);
			
		if(!impl->window)
		{
			XFree(impl->visualinfo);
			throw wm::Exception("Can't create Window");
		}
		// TODO: better error handling, perhaps wait for X 
	}
	
	Window::~Window()
	{
		XFree(impl->visualinfo);
		XDestroyWindow(display().impl->display, impl->window);
	}
	
	void Window::show()
	{
		XMapRaised(display().impl->display, impl->window);
		XFlush(display().impl->display);
		// TODO: Handle errors
	}
	
	void Window::swap()
	{
		glXSwapBuffers(
			display().impl->display,
			impl->window);
	}
	
	void Window::dispatch(bool block)
	{
		long event_mask = impl->event_mask;
		XEvent event;
		
		if(block)
		{
			XWindowEvent(
				display().impl->display,
				impl->window,
				event_mask,
				&event
				);
			{
				boost::scoped_ptr<const Event> ptr(
					xlib::fromXEvent(*this, event));
				if(ptr) impl->dispatcher.dispatch(*ptr);
			}
		} else
		{
			while(XCheckWindowEvent(
				display().impl->display,
				impl->window,
				event_mask,
				&event
				))
			{
				boost::scoped_ptr<const Event> ptr(
					xlib::fromXEvent(*this, event));
				if(ptr) impl->dispatcher.dispatch(*ptr);
			}
	
		}
	}

	common::Dispatcher& Window::dispatcher()
	{
		return impl->dispatcher;
	}
}

