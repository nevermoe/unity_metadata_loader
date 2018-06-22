#if IL2CPP_DEBUGGER_ENABLED

#include "event.h"

#include "../../data-buffer.h"
#include "../../../os/Atomic.h"

using namespace il2cpp::debugger;

static int32_t event_packed_id = 0;

EventCompositeCommand::EventCompositeCommand() :
    Command(kCommandSetEvent, kCommandSetEventComposite, (int32_t)il2cpp::os::Atomic::Increment(&event_packed_id)),
    _suspend_policy(kSuspendPolicyNone)
{
}

EventCompositeCommand::EventCompositeCommand(int32_t /*ignored*/) :
    Command(kCommandSetEvent, kCommandSetEventComposite, (int32_t)il2cpp::os::Atomic::Increment(&event_packed_id)),
    _suspend_policy(kSuspendPolicyNone)
{
}

void EventCompositeCommand::AddEvent(int32_t req_id, const Event *evt)
{
    _events[req_id] = evt;
}

void EventCompositeCommand::WriteContentTo(Buffer &out) const
{
    const size_t count = _events.size();
    std::map<int32_t, const Event*>::const_iterator it = _events.begin();

    // VM_START event suspend policy might be configured by the user, thus allowing
    // the debugger to suspend as soon as the VM start.
    out.WriteByte(_suspend_policy);
    out.WriteInt(count);

    while (it != _events.end())
    {
        it->second->WriteTo(out, it->first);
        ++it;
    }
}

#endif
