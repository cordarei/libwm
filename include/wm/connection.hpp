#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

namespace wm
{
	class Window;
	class EventHandler;
	
	class Connection : boost::noncopyable
	{
		public:
			Connection(Window&, EventHandler&, bool do_connect = true);
			~Connection();

			void connect();
			void disconnect();
			
			bool connected();

		private:
			struct impl_t;
			boost::scoped_ptr<impl_t> impl;
			
			friend class Window;
	};
}

#endif
