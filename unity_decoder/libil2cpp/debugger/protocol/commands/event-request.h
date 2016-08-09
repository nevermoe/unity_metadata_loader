#pragma once

#include <vector>

#include "../../common.h"
#include "../protocol-utils.h"

namespace il2cpp
{
namespace debugger
{

struct EventRequestSetCommand : public CustomCommand<EventRequestSetCommand>
{
	struct Reply : public CustomReply
	{
		Property<int32_t> request_id;

		void WriteContentTo(Buffer &out) const
		{
			request_id.WriteTo(out);
		}
	};


	CUSTOM_COMMAND(EventRequest, Set);

	~EventRequestSetCommand();

	Property<const MethodInfo*> method;
	Property<uint64_t> location;
	Property<Il2CppThread*> thread;
	Property<int32_t> size;
	Property<int32_t> depth;

	inline EventKind kind() const
	{
		return _kind;
	}

	inline SuspendPolicy suspend_policy() const
	{
		return _suspend_policy;
	}

	inline const std::vector<Modifier*> &modifiers() const
	{
		return _modifiers;
	}

	void ReadProperties(Buffer &in);

private:
	
	EventKind _kind;
	SuspendPolicy _suspend_policy;
	std::vector<Modifier*> _modifiers;
};

struct EventRequestClearCommand : public CustomCommand<EventRequestClearCommand>
{
	CUSTOM_COMMAND_EMPTY_REPLY(EventRequest, Clear);

	Property<uint8_t> event_kind;
	Property<int32_t> request_id;

	void ReadProperties(Buffer &in);
};

struct EventRequestClearAllBreakpointsCommand : public CustomCommand<EventRequestClearAllBreakpointsCommand>
{
	CUSTOM_COMMAND_EMPTY_REPLY(EventRequest, ClearAllBreakpoints);
};

}
}
