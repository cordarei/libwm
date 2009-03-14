#ifndef WM_EVENTS_CHARACTEREVENT_HPP
#define WM_EVENTS_CHARACTEREVENT_HPP

#include <boost/cstdint.hpp>

#include <wm/event.hpp>
#include <wm/eventhandler.hpp>

namespace wm
{
	class CharacterEvent : public Event
	{
		public:
			CharacterEvent(
				Window &window,
				boost::uint32_t unicode)
				: Event(window)
				, unicode_(unicode)
			{
			}

			virtual void accept(EventHandler &handler) const { handler.handle(*this); }

			boost::uint32_t unicode() const { return unicode_; }


		private:
			boost::uint32_t unicode_;
	};
}

#endif
