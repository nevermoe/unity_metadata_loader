#pragma once

#include "common.h"

#include <list>
#include <vector>

#include "protocol/globals.h"

struct Il2CppClass;

namespace il2cpp
{
namespace debugger
{

class Event;
class Agent;
struct BreakpointData;
struct SingleStepData;

static int32_t event_request_id;

class EventRequest
{

public:

	EventRequest(EventKind kind, SuspendPolicy suspend_policy, const std::vector<Modifier*> &modifiers);

	inline int32_t id() const
	{
		return _id;
	}

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

	inline bool has_modifiers() const
	{
		return _modifiers.size() > 0;
	}

	inline BreakpointData *breakpoint_data() const
	{
		return _breakpoint_data;
	}

	inline void breakpoint_data(BreakpointData *value)
	{
		_breakpoint_data = value;
	}

	inline SingleStepData *single_step_data() const
	{
		return _single_step_data;
	}

	inline void single_step_data(SingleStepData *value)
	{
		_single_step_data = value;
	}

private:

	int32_t _id;
	EventKind _kind;
	SuspendPolicy _suspend_policy;
	std::vector<Modifier*> _modifiers;
	BreakpointData *_breakpoint_data;
	SingleStepData *_single_step_data;

	DISALLOW_COPY(EventRequest);

	friend class Agent;
};

class EventRequestList
{

public:

	struct FilterInfo
	{
		Il2CppClass *klass;
		Il2CppObject *exception;

		FilterInfo() :
			klass(0),
			exception(0)
		{

		}
	};

	EventRequestList(Agent &agent, bool owns_requests);
	~EventRequestList();

	inline const std::list<const EventRequest*> requests() const
	{
		return _requests;
	}
	
	int32_t RequestIdsFor(std::vector<int32_t> &ids, const Event &evt, FilterInfo &filter) const;
	bool IsClientInterested(const Event &evt) const;
	const EventRequest *GetEventRequestById(int32_t id) const;

	void AddEventRequest(const EventRequest *request);
	const EventRequest* RemoveEventRequest(EventKind kind, int32_t id);

	void ClearAllRequests();

private:

	Agent &_agent;
	bool _owns_requests;
	std::list<const EventRequest*> _requests;

	DISALLOW_COPY(EventRequestList);

};

} /* namespace debugger */
} /* namespace il2cpp */
