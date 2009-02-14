#include <wm/context.hpp>
#include <wm/window.hpp>

namespace wm
{
	struct Context::impl_t
	{
	};

	Context::Context(Window &window, int majorVersion, int minorVersion)
		: impl(0)
		, display_(window.display())
	{
	}

	Context::Context(Window &window, Context& sharedContext, int majorVersion, int minorVersion)
		: impl(0)
		, display_(window.display())
	{
	}
	
	Context::~Context()
	{
	}

	void makeCurrent(Window &, Context &)
	{
	}
}
