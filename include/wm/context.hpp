#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

namespace wm
{
	class Window;
	class Display;

	class Context
	{
		public:
			Context(Window &window, int majorVersion, int minorVersion);
			Context(Window &window, Context& sharedContext, int majorVersion, int minorVersion);
			~Context();
			
			Display& display() { return display_; }
			
		private:
			struct impl_t;
			boost::scoped_ptr<impl_t> impl;
			
			Display &display_;
			friend void makeCurrent(Window&, Context&);
	};
}

#endif

