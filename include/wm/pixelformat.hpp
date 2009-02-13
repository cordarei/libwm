#ifndef PIXELFORMAT_HPP
#define PIXELFORMAT_HPP

namespace wm
{
	class PixelFormat // noncopyable
	{
		public:
			enum /* class */ DataType {
				UNSIGNED_BYTE,
				UNSIGNED_SHORT,
				UNSIGNED_INT,
				FLOAT
				/*, HALF_FLOAT */
				};
		
			explicit PixelFormat(
				int red = 8,		// color buffer size
				int green = 8,
				int blue = 8,
				int alpha = 8,
				int depth = 24,		// depth-stencil buffer size
				int stencil = 8,
				int samples = 4,	// multisample samples
				int buffers = 1,	// multisample buffers
				bool srgb = false,
				DataType type = UNSIGNED_BYTE );
				
		private:
	};
}

#endif

