#pragma once

#include "../../common.h"
#include "../../meta.h"
#include "../protocol.h"

namespace il2cpp
{
namespace debugger
{

class EventCompositeCommand : public Command
{

public:
	
	EventCompositeCommand();
	explicit EventCompositeCommand(int32_t /*ignored*/);

	inline SuspendPolicy suspend_policy() const
	{
		return _suspend_policy;
	}

	inline void suspend_policy(SuspendPolicy v)
	{
		_suspend_policy = v;
	}

	void AddEvent(int32_t req_id, const Event *evt);

protected:
	
	void WriteContentTo(Buffer &out) const;

private:

	SuspendPolicy _suspend_policy;
	std::map<int32_t, const Event*> _events;

	PROTOCOL_COMMAND(Event, Composite);

	DISALLOW_COPY(EventCompositeCommand);
};

}
}
