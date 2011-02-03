#ifndef WM_OSTREAM_OUTPUT_HPP
#define WM_OSTREAM_OUTPUT_HPP

#include <iostream>
#include <cctype>

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
        int (*isprint)(int) = &std::isprint; // use the C isprint, not the C++ one with locales

        switch(event.type)
        {
            case NO_EVENT:
                return os << "NO_EVENT";
            case KEY_DOWN:
            case KEY_UP:
                os << ((event.type == KEY_UP) ? "KEY_UP" : "KEY_DOWN")
                    << "  window: " << event.any.window
                    << "  symbol: " << keyboard::keyName(event.key.symbol)
                    << "  keymod: ";
                util::printKeyMod(os, event.key.keymod);
                os << "  repeat: " << event.key.repeat;
                return os;
            case BUTTON_DOWN:
            case BUTTON_UP:
                os << ((event.type == BUTTON_UP) ? "BUTTON_UP" : "BUTTON_DOWN")
                    << "  window: " << event.any.window
                    << "  x: " << event.button.x
                    << "  y: " << event.button.y
                    << "  button: " << mouse::buttonName(event.button.button)
                    << "  buttons: ";
                util::printButtonMask(os, event.button.buttons);
                os << "  keymod: ";
                util::printKeyMod(os, event.button.keymod);
                return os;
            case TEXT_INPUT:
                os << "TEXT_INPUT"
                    << "  window: " << event.any.window
                    << "  unicode: " << event.text.unicode;
                if(event.text.unicode < 128 && isprint(event.text.unicode))
                    os << "  character: "<< char(event.text.unicode);
                return os;
            case EXPOSE:
                return os << "EXPOSE"
                    << "  window: " << event.any.window
                    << "  x: " << event.expose.x
                    << "  y: " << event.expose.y
                    << "  width: " << event.expose.width
                    << "  height: " << event.expose.height;
            case MOTION:
                os << "MOTION"
                    << "  window: " << event.any.window
                    << "  x: " << event.motion.x
                    << "  y: " << event.motion.y
                    << "  buttons: ";
                util::printButtonMask(os, event.motion.buttons);
                os << "  keymod: ";
                util::printKeyMod(os, event.motion.keymod);
                return os;
            case MOUSE_ENTER:
            case MOUSE_LEAVE:
                return os << ((event.type == MOUSE_ENTER) ? "MOUSE_ENTER" : "MOUSE_LEAVE")
                    << "  window: " << event.any.window
                    << "  x: " << event.mouseover.x
                    << "  y: " << event.mouseover.y;
            case RESIZE:
                return os << "RESIZE"
                    << "  window: " << event.any.window
                    << "  width: " << event.resize.width
                    << "  height: " << event.resize.height;
            case CLOSE:
                return os << "CLOSE"
                    << "  window: " << event.any.window;
            case FOCUS_GOT:
            case FOCUS_LOST:
                return os << ((event.type == FOCUS_GOT) ? "FOCUS_GOT" : "FOCUS_LOST")
                    << "  window: " << event.any.window;
            case SHOW:
            case HIDE:
                return os << ((event.type == SHOW) ? "SHOW" : "HIDE")
                    << "  window: " << event.any.window;
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
