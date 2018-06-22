#pragma once

#include "os/ErrorCodes.h"
#include "os/Handle.h"
#include "os/WaitStatus.h"
#include "utils/NonCopyable.h"

namespace il2cpp
{
namespace os
{

class EventImpl;

class Event : public il2cpp::utils::NonCopyable
{
public:
	Event (bool manualReset = false, bool signaled = false);
	~Event ();

	ErrorCode Set ();
	ErrorCode Reset ();
	WaitStatus Wait (bool interruptible = false);
	WaitStatus Wait (uint32_t ms, bool interruptible = false);

private:
	EventImpl* m_Event;
};

class EventHandle : public Handle
{
public:
	EventHandle (Event* event)
		: m_Event (event) {}

	virtual ~EventHandle () { delete m_Event; };
	virtual bool Wait () { m_Event->Wait (true); return true; }
	virtual bool Wait (uint32_t ms) { return m_Event->Wait (ms, true) != kWaitStatusTimeout; }
	virtual void Signal () { m_Event->Set (); }
	Event& Get() { return *m_Event; }

private:
	Event* m_Event;
};

}
}
