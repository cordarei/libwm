#ifndef CONTEXT_HPP
#define CONTEXT_HPP

namespace wm
{
	class Context
	{
		public:
			Context(Window &window, int majorVersion, int minorVersion);
			Context(Window &window, Context& sharedContext, int majorVersion, int minorVersion);
		private:
	};
}

#endif

