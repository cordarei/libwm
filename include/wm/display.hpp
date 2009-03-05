#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

namespace wm
{
	class Window;
	class Context;

	class Display : boost::noncopyable
	{
		public:
			explicit Display(const char *name = 0);
			~Display();
			
			void dispatch(bool block);
			
		private:
			struct impl_t;
			boost::scoped_ptr<impl_t> impl;
			
			friend class EventReader;
			friend class Window;
			friend class Context;
			
			friend void makeCurrent(Window&, Context&);
	};
}

#endif


