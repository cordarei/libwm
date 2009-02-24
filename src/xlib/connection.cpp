#include <iostream>

#include <wm/connection.hpp>
#include <wm/window.hpp>

#include <common/dispatcher.hpp>

#include "impl/window_impl.hpp"

namespace wm
{
	struct Connection::impl_t : boost::noncopyable
	{
		explicit impl_t(common::Dispatcher& dispatcher, EventHandler &handler)
			: dispatcher(dispatcher)
			, handler(handler)
		{
		}
	
		common::Dispatcher &dispatcher;
		EventHandler &handler;
		common::Dispatcher::ConnectionInfo info;
	};

	Connection::Connection(Window& window, EventHandler &handler, bool do_connect)
		: impl(new impl_t(window.impl->dispatcher, handler))
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
		impl->dispatcher.connect(impl->handler, impl->info);
	}

	void Connection::disconnect()
	{
		impl->dispatcher.disconnect(impl->info);
	}
	
	bool Connection::connected()
	{
		return impl->dispatcher.isConnected(impl->info);
	}
}

