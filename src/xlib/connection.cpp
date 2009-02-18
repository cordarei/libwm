#include <iostream>

#include <wm/connection.hpp>
#include <wm/window.hpp>

#include "impl/window_impl.hpp"

namespace wm
{
	struct Connection::impl_t : boost::noncopyable
	{
		explicit impl_t(Window& window, EventHandler &handler)
			: window(window)
			, handler(handler)
		{
		}
	
		Window &window;
		EventHandler &handler;
		Window::ConnectionInfo info;
	};

	Connection::Connection(Window& window, EventHandler &handler, bool do_connect)
		: impl(new impl_t(window, handler))
	{
		if(do_connect) connect();
	}
	
	Connection::~Connection()
	{
		try
		{
			disconnect();
		} catch(std::exception &exc)
		{
			std::cerr << "Can't disconnect EventHandler from Window: " << exc.what() << std::endl;
		} catch(...)
		{
			std::cerr << "Can't disconnect EventHandler from Window" << std::endl;
		}
	}
	
	void Connection::connect()
	{
		impl->window.connect(impl->handler, impl->info);
	}

	void Connection::disconnect()
	{
		impl->window.disconnect(impl->info);
	}
	
	bool Connection::connected()
	{
		return impl->window.isConnected(impl->info);
	}
}

