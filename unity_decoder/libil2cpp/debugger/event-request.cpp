#if IL2CPP_DEBUGGER_ENABLED

#include "event-request.h"

#include "../os/Atomic.h"
#include "../il2cpp-api.h"

#include "protocol/protocol.h"

using namespace il2cpp::debugger;

EventRequest::EventRequest(EventKind kind, SuspendPolicy suspend_policy, const std::vector<Modifier*> &modifiers) :
    _id((int32_t)il2cpp::os::Atomic::Increment(&event_request_id)),
    _kind(kind),
    _suspend_policy(suspend_policy),
    _modifiers(modifiers),
    _breakpoint_data(0),
    _single_step_data(0)
{
}

EventRequestList::EventRequestList(Agent &agent, bool owns_requests) :
    _agent(agent),
    _owns_requests(owns_requests)
{
}

EventRequestList::~EventRequestList()
{
    if (!_owns_requests)
        return;

    ClearAllRequests();
}

void EventRequestList::ClearAllRequests()
{
    IL2CPP_ASSERT(_owns_requests && "EventRequestList::ClearAllRequests cannot be invoked on a list that does not own the requests");

    std::list<const EventRequest*>::const_iterator it = _requests.begin();
    while (it != _requests.end())
    {
        delete (*it);
        ++it;
    }

    _requests.clear();
}

bool EventRequestList::IsClientInterested(const Event &evt) const
{
    if (evt.kind() == kEventKindVmStart || evt.kind() == kEventKindVmDeath)
        return true;

    std::list<const EventRequest*>::const_iterator it = _requests.begin();
    while (it != _requests.end())
    {
        if ((*it)->kind() == evt.kind())
            return true;

        ++it;
    }

    return false;
}

int32_t EventRequestList::RequestIdsFor(std::vector<int32_t> &ids, const Event &evt,  FilterInfo &filter) const
{
    if (evt.kind() == kEventKindVmStart || evt.kind() == kEventKindVmDeath)
        ids.push_back(0);
    else
    {
        std::list<const EventRequest*>::const_iterator it = _requests.begin();
        while (it != _requests.end())
        {
            const EventRequest* req = *it;
            if (req->kind() == evt.kind())
            {
                bool filtered = false;
                std::vector<Modifier*>::const_iterator mit = req->modifiers().begin();

                while (mit != req->modifiers().end())
                {
                    Modifier *modifier = *mit;

                    switch (modifier->kind)
                    {
                        case kModifierKindCount:
                            filtered = ((modifier->data.count > 0) && (--modifier->data.count > 0));
                            break;

                        case kModifierKindLocationOnly:
                            IL2CPP_ASSERT(0 && "kModifierKindLocationOnly modifier filter not supported!");
                            break;

                        case kModifierKindStep:
                            IL2CPP_ASSERT(0 && "kModifierKindStep modifier filter not supported!");
                            break;

                        case kModifierKindThreadOnly:
                            filtered = (modifier->data.thread != il2cpp_thread_current());
                            break;

                        case kModifierKindExceptionOnly:
                            IL2CPP_ASSERT(0 && "kModifierKindExceptionOnly modifier filter not supported!");
                            break;

                        case kModifierKindAssemblyOnly:
                            IL2CPP_ASSERT(0 && "kModifierKindAssemblyOnly modifier filter not supported!");
//              int k;
//              gboolean found = FALSE;
//              MonoAssembly **assemblies = mod->data.assemblies;

//              if (assemblies) {
//                  for (k = 0; assemblies [k]; ++k)
//                      if (assemblies [k] == klass->image->assembly)
//                          found = TRUE;
//              }
//              if (!found)
//                  filtered = TRUE;
                            break;
                    }

                    ++mit;
                }

                if (!filtered)
                    ids.push_back(req->id());
            }

            ++it;
        }
    }

    return ids.size();
}

const EventRequest *EventRequestList::GetEventRequestById(int32_t id) const
{
    std::list<const EventRequest*>::const_iterator it = _requests.begin();
    while (it != _requests.end())
    {
        if ((*it)->id() == id)
            return *it;

        ++it;
    }

    return NULL;
}

void EventRequestList::AddEventRequest(const EventRequest *request)
{
    _requests.push_back(request);
}

const EventRequest* EventRequestList::RemoveEventRequest(EventKind kind, int32_t id)
{
    std::list<const EventRequest*>::iterator it = _requests.begin();
    while (it != _requests.end())
    {
        if ((*it)->kind() == kind && (*it)->id() == id)
        {
            const EventRequest* req = *it;
            _requests.erase(it);
            return req;
        }

        ++it;
    }

    return NULL;
}

#endif
