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
	
	/// A Connection from a Window to an EventHandler
	class WM_EXPORT Connection
	{
		public:
			/// Create a new connection from a Window to an EventHandler
			/**
				Creates a new Connection from a Window to an EventHandler.
				When a Window dispatches events, it calls all event handlers
				connected to the Window.
				
				A connection can be disconnected and reconnected.
				
				In some situations the connection should be disconnected
				before the Connection object is destroyed by destructors.
			
				@param window the Window to attach to
				@param handler the EventHandler to attach
				@param do_connect true if the constructor should connect the connection
				@see Window::dispatch
			*/
			Connection(Window& window, EventHandler& handler, bool do_connect = true);
			
			/// Disconnect and destroy Connection
			/**
				If the connection is still in a connected state, the
				destructor will disconnect the connection.
				
				@see disconnect
			*/
			~Connection();

			/// Connect a connection
			void connect();
			
			/// Disconnect a connection
			void disconnect();
			
			/// Check if a connection is connected
			/**
				@return true if the connection is connected
			*/
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
