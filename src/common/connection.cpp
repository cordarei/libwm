#include <iostream>

#include <wm/connection.hpp>
#include <wm/window.hpp>

#include <common/dispatcher.hpp>

namespace wm
{
	Connection::Connection(Window& window, EventHandler& handler, bool do_connect)
		: window(window)
		, handler(handler)
		, info(0)
	{
		if(do_connect) connect();
	}
	
	Connection::~Connection()
	{
		try
		{
			disconnect();
		} catch(std::exception &e)
		{
			std::cerr << "Can't disconnect event handler: " << e.what() << std::endl;
		} catch(...)
		{
			std::cerr << "Can't disconnect event handler" << std::endl;
		}
	}

	void Connection::connect()
	{
		if(!info)
			info = window.dispatcher().connect(handler);
	}
	
	void Connection::disconnect()
	{
		if(info)
		{
			window.dispatcher().disconnect(info);
			info = 0;
		}
	}
	
	bool Connection::connected()
	{
		return info ? true : false;
	}
}

