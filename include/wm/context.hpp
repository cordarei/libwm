#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <wm/export.hpp>

namespace wm
{
	class Window;
	class Display;
	class Surface;

	/// A rendering context
	class WM_EXPORT Context
	{
		public:
			/// Create a new rendering context
			/**
				@param format the pixel format to use with the rendering context
				@param versionMajor major OpenGL version of the context
				@param versionMinor minor OpenGL version of the context
				@param compatible enable OpenGL backward compatibility
				@param debug create a debugging context
				@param direct attempt to use direct rendering
				@param shared a pointer to a Context to share resources
					(buffers, textures, shaders, etc) with or 0 for 
					no shared context
				@see Context::direct
			*/
			explicit Context(
				const PixelFormat &format,
				int versionMajor = 1,
				int versionMinor = 0,
				bool compatible = true,
				bool debug = false,
				bool direct = true,
				Context *shared = 0);
			
			/// Destroy a rendering context
			~Context();
			
			/// Get the display associated with this rendering context
			Display& display() const { return *display_; }

			/// Check if the context supports direct rendering
			/**
				@return true if the direct rendering is supported
					or the underlying platform doesn't distinguish indirect rendering
			*/
			bool direct() const;
			
		private:
			Context(const Context&);
			Context& operator=(const Context&);

			struct impl_t;
			impl_t* impl;
			
			Display *display_;

			friend class CurrentContext;
	};

	/// A RAII Wrapper for manipulating current thread-local contexts
	class WM_EXPORT CurrentContext
	{
		public:
			/// Create a new CurrentContext and optionally set the current thread-local context
			/**
				@param context the Context to use
				@param draw the rendering Surface to draw to
				@param read the rendering Surface to read from
				@param do_setup true to set the current thread-local context
				@see setup
				@see release
				@see reset
			*/
			CurrentContext(Context &context, Surface &draw, Surface &read, bool do_setup = true);
			
			/// Create a new CurrentContext and optionally set the current thread-local context
			/**
				@param context the Context to use
				@param drawread the rendering Surface to draw to and read from
				@param do_setup true to set the current thread-local context
				@see setup
				@see release
				@see reset
			*/
			CurrentContext(Context &context, Surface &drawread, bool do_setup = true);
			
			/// Destroy CurrentContext and restore previously bound context
			/**
				@see release
				@see reset
			*/
			~CurrentContext();

			/// Set the current thread-local context
			/**
				@see release
				@see reset
			*/
			void setup();
			
			/// Restore previous context
			/**
				@see release
				@see setup
			*/
			void reset();
			
			/// Release the CurrentContext to disable restoring previous context
			/**
				Calling release will inhibit the CurrentContext destructor from
				restoring the previously bound context. Use the reset function to
				manually restore previous context.
				
				@see setup
				@see reset
			*/
			void release();
			
			static void swapInterval(Configuration& config, int interval);
			
			void swapInterval(int interval);

		private:
			CurrentContext(const CurrentContext&);
			CurrentContext& operator=(const CurrentContext&);

			Context &context;
			Surface &draw;
			Surface &read;

			bool do_reset;
			unsigned int num1, num2, num3;
			void *ptr1, *ptr2, *ptr3;
	};
}

#undef WM_EXPORT

#endif

