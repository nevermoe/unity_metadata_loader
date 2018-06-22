#if IL2CPP_DEBUGGER_ENABLED

#pragma once

#include "../../il2cpp-api.h"

#include "../agent.h"
#include "../protocol/commands/event-request.h"
#include "../protocol/commands/internal-error.h"

namespace il2cpp
{
namespace debugger
{
    const Reply *Agent::Process(const EventRequestSetCommand *command)
    {
        SuspendPolicy suspend_policy = command->suspend_policy();

        // We always suspend in case of breakpoint or single step events
        if (command->kind() == kEventKindBreakpoint || command->kind() == kEventKindStep)
            suspend_policy = kSuspendPolicyAll;

        EventRequest *request = new EventRequest(
                command->kind(),
                suspend_policy,
                command->modifiers());

        // TODO: if something was wrong, please make sure to set reply->error(...)

        _event_req_list.AddEventRequest(request);

        switch (command->kind())
        {
            case kEventKindAssemblyLoad:
            case kEventKindAssemblyUnload:
            case kEventKindThreadStart:
            case kEventKindThreadDeath:
            case kEventKindUserBreak:
            case kEventKindUserLog:
            case kEventKindException:
            case kEventKindVmDeath:
            case kEventKindVmStart:
            case kEventKindAppDomainCreate:
            case kEventKindAppDomainUnload:
                // Nothing special to do here
                break;

            case kEventKindTypeLoad:
                // Note: this should be set to true only if defer=y
                _notify_pending_events_on_first_invoke = true;
                break;

            case kEventKindBreakpoint:
            {
                IL2CPP_ASSERT(command->method() != 0 && "kEventKindBreakpoint expects a method and a location to be set!");

                BreakpointData *data = new BreakpointData();
                data->method = command->method();
                data->request = request;
                data->location = command->location();

                _bp_data.push_back(data);

                request->breakpoint_data(data);

                il2cpp_debug_method_set_breakpoint_data_at(
                    (Il2CppDebugMethodInfo*)il2cpp_debug_get_method_info(command->method()),
                    command->location(),
                    data);
            }
            break;

            case kEventKindStep:
            {
                IL2CPP_ASSERT(command->thread() != 0 && "kEventKindStep expects a thread, size and depth to be set!");

                // TODO: we probably need to clear also the SingleStepData state, if any.
                if (_ss_data != 0)
                    ClearSingleStepData();

                _ss_data = PrepareSingleStepData(
                        request, command->thread(),
                        (StepSizeKind)command->size(),
                        (StepDepthKind)command->depth());

                request->single_step_data(_ss_data);
            }
            break;

            case kEventKindMethodEntry:
                /*req->info = set_breakpoint (NULL, METHOD_ENTRY_IL_OFFSET, req);*/

                IL2CPP_ASSERT(0 && "Not implemented request kEventKindMethodEntry");
                break;

            case kEventKindMethodExit:
                /*req->info = set_breakpoint (NULL, METHOD_EXIT_IL_OFFSET, req);*/

                IL2CPP_ASSERT(0 && "Not implemented request kEventKindMethodExit");
                break;

            case kEventKindKeepAlive:
                // Was empty

                IL2CPP_ASSERT(0 && "Not implemented request kEventKindKeepAlive");
                break;

            default:
                LOG("Not implemented request ??(" << command->kind() << ") with modifiers");
                IL2CPP_ASSERT(0 && "Not implemented request");
        }

        EventRequestSetCommand::Reply *event_request_set_reply = command->reply();
        event_request_set_reply->request_id(request->id());

        /*LOG("NotifyPendingEventsOfKind is being invoked from the debugger agent thread, which is WRONG. Only the VM thread should send these events.");
        NotifyPendingEventsOfKind(command->kind());*/

        return event_request_set_reply;
    }

    const Reply *Agent::Process(const EventRequestClearCommand *command)
    {
        EventRequestClearCommand::Reply *empty_reply = command->reply();

        const EventRequest *request = _event_req_list.RemoveEventRequest(
                (EventKind)command->event_kind(),
                command->request_id());

        if (request == 0)
        {
            LOG("EventRequestClearCommand: invalid request Id: " << command->request_id());
            return empty_reply;
        }

        BreakpointData *data = request->breakpoint_data();
        if (data != 0)
        {
            LOG("EventRequestClearCommand: breakpoint_data found, deleting it");

            _bp_data.remove(data);

            il2cpp_debug_method_clear_breakpoint_data_at(
                (Il2CppDebugMethodInfo*)il2cpp_debug_get_method_info(data->method),
                data->location);

            delete data;
        }

        delete request;

        return empty_reply;
    }

    const Reply *Agent::Process(const EventRequestClearAllBreakpointsCommand *command)
    {
        EventRequestClearAllBreakpointsCommand::Reply *empty_reply = command->reply();

        std::list<BreakpointData*>::iterator it = _bp_data.begin();

        while (it != _bp_data.end())
        {
            BreakpointData* data = *it;

            il2cpp_debug_method_clear_breakpoint_data(
                (Il2CppDebugMethodInfo*)il2cpp_debug_get_method_info(data->method));

            delete data;

            ++it;
        }

        _bp_data.clear();

        return empty_reply;
    }
}
}

#endif
