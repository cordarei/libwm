#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <wm/export.hpp>

namespace wm
{
	class Window;
	class EventHandler;
	
	namespace common
	{
		struct ConnectionInfo;
	}
	
	class WM_EXPORT Connection
	{
		public:
			Connection(Window&, EventHandler&, bool do_connect = true);
			~Connection();

			void connect();
			void disconnect();
			
			bool connected();

		private:
			Connection(const Connection&);
			Connection& operator=(const Connection&);

			Window &window;
			EventHandler &handler;
			common::ConnectionInfo *info;			
	};
}

#undef WM_EXPORT

#endif
