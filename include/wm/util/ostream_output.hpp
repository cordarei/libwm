#ifndef WM_OSTREAM_OUTPUT_HPP
#define WM_OSTREAM_OUTPUT_HPP

#include <iostream>

#include <wm/pixelformat.hpp>
#include <wm/configuration.hpp>
#include <wm/exception.hpp>

namespace wm
{
	namespace util
    {
        inline const char *pixelDataType2String(PixelFormat::DataType type)
        {
            switch(type)
            {
                case PixelFormat::INTEGER: return "Integer";
                case PixelFormat::FLOAT: return "Float";
                case PixelFormat::UNSIGNED_FLOAT: return "Unsigned float";
            }

            throw wm::Exception("Invalid color buffer data type");
        }

        std::ostream &printKeyMod(std::ostream& os, wm::keyboard::KeyMod mod)
        {
            return os << ""
                << ((mod & wm::keyboard::MOD_SHIFT) ? "Shift " : "")
                << ((mod & wm::keyboard::MOD_CONTROL) ? "Control " : "")
                << ((mod & wm::keyboard::MOD_ALT) ? "Alt " : "")
                << ((mod & wm::keyboard::MOD_SUPER) ? "Super " : "")
                << ((mod & wm::keyboard::MOD_ALTGR) ? "AltGr " : "")
                << ((mod & wm::keyboard::MOD_NUMLOCK) ? "Num Lock " : "")
                << ((mod & wm::keyboard::MOD_CAPSLOCK) ? "Caps Lock " : "")
                << ((!mod) ? "None " : "")
                ;
        }

        std::ostream &printButtonMask(std::ostream &os, wm::mouse::ButtonMask mask)
        {
            return os << ""
                << ((mask & wm::mouse::MASK_LEFT) ? "Left " : "")
                << ((mask & wm::mouse::MASK_MIDDLE) ? "Middle " : "")
                << ((mask & wm::mouse::MASK_RIGHT) ? "Right " : "")
                << ((!mask) ? "None " : "")
                ;
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
        switch(event.type)
        {
            case NO_EVENT:
                return os << "NO_EVENT";
            case KEY_DOWN:
            case KEY_UP:
                return os
                    << ((event.type == KEY_UP) ? "KEY_UP" : "KEY_DOWN");
            case BUTTON_DOWN:
            case BUTTON_UP:
                return os
                    << ((event.type == BUTTON_UP) ? "BUTTON_UP" : "BUTTON_DOWN");
            case TEXT_INPUT:
                return os << "TEXT_INPUT";
            case EXPOSE:
                return os << "EXPOSE";
            case MOTION:
                return os << "MOTION";
            case MOUSE_ENTER:
            case MOUSE_LEAVE:
                return os << "MOUSE_LEAVE";
            case RESIZE:
                return os << "RESIZE";
            case CLOSE:
                return os << "CLOSE";
            case FOCUS_GOT:
            case FOCUS_LOST:
                return os
                    << ((event.type == FOCUS_GOT) ? "FOCUS_GOT" : "FOCUS_LOST");
            case SHOW:
            case HIDE:
                return os
                    << ((event.type == SHOW) ? "SHOW" : "HIDE");
            default:
                return os;
        }
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
			<< "  type: " << util::pixelDataType2String(descriptor.type)
			<< (descriptor.srgb ? "  sRGB" : "")
			<< (descriptor.slow ? "  Slow" : "")
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
