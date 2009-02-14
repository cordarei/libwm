#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

namespace wm
{
	class Display : boost::noncopyable
	{
		public:
			explicit Display(const char *name = 0);
			~Display();
			
		private:
			struct impl_t;
			boost::scoped_ptr<impl_t> impl;
			
			friend class Window;
	};
}

#endif

