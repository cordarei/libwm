#ifndef PIXELFORMAT_HPP
#define PIXELFORMAT_HPP

#include <wm/exception.hpp>
#include <wm/configuration.hpp>

#include <wm/export.hpp>

namespace wm
{
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
					@param samples the number of multisampling samples
					@param buffers the number of multisampling buffers
				*/
				Descriptor(
					int red,
					int green,
					int blue,
					int alpha,
					int depth,
					int stencil,
					int samples = 0,
					int buffers = 0)
					: red(red)
					, green(green)
					, blue(blue)
					, alpha(alpha)
					, depth(depth)
					, stencil(stencil)
					, samples(samples)
					, buffers(buffers)
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
				
				/// the number of multisampling samples
				int samples;
				/// the number of multisampling buffers
				int buffers;
			};

			/// Get the descriptor of this pixel format
			/**
				@return a reference to the descriptor of this pixel format
			*/
			const Descriptor &descriptor() const { return descriptor_; }
			
			/// Get the Configuration object that owns this pixel format
			/**
				@return a reference to the Configuration object that owns this pixel format
			*/
			Configuration& configuration() const { return *configuration_; }
			
		private:
			struct impl_t;
			impl_t *impl;
			Configuration *configuration_;
			Descriptor descriptor_;
			
			PixelFormat(const Descriptor& descriptor, Configuration& configuration, impl_t &data)
				: impl(&data)
				, configuration_(&configuration)
				, descriptor_(descriptor)
			{
			}

			void set(Window& win) const;
			
			friend class Configuration;
			friend struct Configuration::impl_t;
			friend class Surface;
			friend class Context;
			friend class Window;
	};
	
	/// Check if two pixel format descriptors match exactly
	/**
		@param d1 first pixel format for comparison
		@param d2 second pixel format for comparison
		@return true if d1 and d2 match exactly
	*/
	inline bool match(const PixelFormat::Descriptor& d1, const PixelFormat::Descriptor& d2)
	{
		return true
			&& d1.red == d2.red
			&& d1.green == d2.green
			&& d1.blue == d2.blue
			&& d1.alpha == d2.alpha
			&& d1.depth == d2.depth
			&& d1.stencil == d2.stencil
			&& d1.samples == d2.samples
			&& d1.buffers == d2.buffers
			;			
	}
	
	/// Check if two pixel formats are compatible
	/**
		@param desc the pixel format descriptor tested for compatibility
		@param reference the reference pixel format descriptor
		@return true if pixel format described by desc is compatible with reference
	*/	
	inline bool compatible(const PixelFormat::Descriptor& desc, const PixelFormat::Descriptor& reference)
	{
		return true
			&& desc.red >= reference.red
			&& desc.green >= reference.green
			&& desc.blue >= reference.blue
			&& desc.alpha >= reference.alpha
			&& desc.depth >= reference.depth
			&& desc.stencil >= reference.stencil
			&& desc.samples >= reference.samples
			&& desc.buffers >= reference.buffers
			;
	}
	
	/// Select the better of two compatible pixel format descriptors
	/**
		@param d1 the first compared pixel format descriptor
		@param d2 the second compared pixel format descriptor
		@return true if d1 is a better match
	*/
	inline bool select(const PixelFormat::Descriptor& d1, const PixelFormat::Descriptor& d2)
	{
		return false
			|| d1.red > d2.red
			|| d1.green > d2.green
			|| d1.blue > d2.blue
			|| d1.alpha > d2.alpha
			|| d1.depth > d2.depth
			|| d1.stencil < d2.stencil	// select the one with smaller stencil value like glxChooseFBConfig
			|| d1.samples < d2.samples
			|| d1.buffers < d2.buffers
			;
	}
	
	/// Choose a matching pixel format from a Configuration
	/**
		@param config the graphics configuration where to select from
		@param desc a pixel format descriptor of the desired format
		@return a reference to the selected pixel format
		@throws wm::Exception if no suitable pixel formats are found
	*/	
	inline const PixelFormat& choose(const Configuration& config, const PixelFormat::Descriptor &desc)
	{
		const PixelFormat *best = 0;
		for(int i = 0; i < config.numFormats(); ++i)
		{
			const PixelFormat& format = config.getFormat(i);
			
			if(match(format.descriptor(), desc)) return format;
			
			if(compatible(format.descriptor(), desc))
			{
				if(!best) best = &format;
				else
				{
					if(select(format.descriptor(), best->descriptor()))
						best = &format;
				}
			}
		}
		
		if(!best) throw wm::Exception("Can't choose PixelFormat, no suitable formats available");
		
		return *best;
	}
	
	/// Choose a pixel format
	/**
		@param config the configuration to select from
		@param r number of red bits in the color buffer
		@param g number of green bits in the color buffer
		@param b number of blue bits in the color buffer
		@param a number of alpha bits in the color buffer
		@param depth number of bits in the depth buffer
		@param stencil number of bits in the stencil buffer
		@param samples the number of multisampling samples
		@param buffers the number of multisampling buffers
		@return a reference to a PixelFormat owned by the Configuration object
	*/	
	inline const PixelFormat& choose(const Configuration& config,
		int r = 0, int g = 0, int b = 0, int a = 0,
		int depth = 0, int stencil = 0,
		int samples = 0, int buffers = 0
		)
	{
		return choose(config, PixelFormat::Descriptor(r, g, b, a, depth, stencil, samples, buffers));
	}
}

#undef WM_EXPORT

#endif

