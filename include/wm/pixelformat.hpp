#ifndef PIXELFORMAT_HPP
#define PIXELFORMAT_HPP

#include <wm/export.hpp>

namespace wm
{
	class Configuration;

	/// A pixel format
	class WM_EXPORT PixelFormat
	{
		public:
			/// Pixel format descriptor
			struct WM_EXPORT Descriptor
			{
				/// Create a new pixel format descriptor
				/**
					@param red the number of red bits in the color buffer
					@param green the number of green bits in the color buffer
					@param blue the number of blue bits in the color buffer
					@param alpha the number of alpha bits in the color buffer
					@param depth the number of bits in the depth buffer
					@param stencil the number of bits in the stencil buffer
				*/
				Descriptor(int red, int green, int blue, int alpha, int depth, int stencil)
					: red(red)
					, green(green)
					, blue(blue)
					, alpha(alpha)
					, depth(depth)
					, stencil(stencil)
				{
				}
					
				/// the number of red bits in the color buffer
				int red;
				/// the number of green bits in the color buffer
				int green;
				/// the number of blue bits in the color buffer
				int blue;
				/// the number of alpha bits in the color buffer
				int alpha;
				
				/// the number of bits in the depth buffer
				int depth;
				/// the number of bits in the stencil buffer
				int stencil;
			};

			/// Get the descriptor of this pixel format
			/**
				@return a reference to the descriptor of this pixel format
			*/
			const Descriptor &descriptor() const { return descriptor_; }
			
		private:
			struct impl_t;
			impl_t *impl;
			Configuration *configuration;
			Descriptor descriptor_;
			
			PixelFormat(const Descriptor& descriptor, Configuration& configuration, impl_t &data)
				: impl(&data)
				, configuration(&configuration)
				, descriptor_(descriptor)
			{
			}
			
			friend class Configuration;
			friend class Surface;
			friend class Context;
	};
}

#undef WM_EXPORT

#endif

