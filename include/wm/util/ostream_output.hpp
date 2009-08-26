#ifndef WM_OSTREAM_OUTPUT_HPP
#define WM_OSTREAM_OUTPUT_HPP

#include <iostream>

#include <wm/pixelformat.hpp>
#include <wm/configuration.hpp>

#include <wm/util/eventprinter.hpp>

namespace wm
{
	namespace util
	{
		namespace detail
		{
			inline const char *pixelDataType2String(PixelFormat::DataType type)
			{
				switch(type)
				{
					case PixelFormat::INTEGER: return "Integer";
					case PixelFormat::FLOAT: return "Float";
					case PixelFormat::UNSIGNED_FLOAT: return "Unsigned float";
				}
			}
		}
	}

	/// Write an event to an output stream
	/**
		@param os the output stream to write to
		@param event the event to write
		@return the output stream
	*/
	std::ostream& operator<<(std::ostream& os, const Event& event)
	{
		util::EventPrinter handler(os, "");
		event.accept(handler);
		return os;	
	}
	
	/// Write a pixel format descriptor to an output stream
	/**
		@param os the output stream to write to
		@param descriptor the pixel format descriptor to write
		@return the output stream
	*/
	std::ostream& operator<<(std::ostream& os, const PixelFormat::Descriptor& descriptor)
	{
		return os << "PixelFormat"
			<< "  r: " << descriptor.red
			<< "  g: " << descriptor.green
			<< "  b: " << descriptor.blue
			<< "  a: " << descriptor.alpha
			<< "  depth: " << descriptor.depth
			<< "  stencil: " << descriptor.stencil
			<< "  samples: " << descriptor.samples
			<< "  buffers: " << descriptor.buffers
			<< "  type: " << util::detail::pixelDataType2String(descriptor.type)
			<< (descriptor.srgb ? "  sRGB" : "");
			;
	}
	
	/// Write a graphics configuration to an output stream
	/**
		@param os the output stream to write to
		@param config the graphics configuration to write
		@return the output stream
	*/
	std::ostream& operator<<(std::ostream& os, const Configuration& config)
	{
		int num = config.numFormats();
		os << num << " pixel formats" << std::endl;
		
		for(int i = 0; i < num; ++i)
		{
			os << config.getFormat(i).descriptor() << std::endl;
		}
		
		return os;
	}	
}

#endif
