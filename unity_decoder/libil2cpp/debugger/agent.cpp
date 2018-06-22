#if IL2CPP_DEBUGGER_ENABLED

#include "agent.h"

#include "id-bank.h"
#include "transport-tcp.h"
#include "protocol/protocol.h"
#include "protocol/events.h"
#include "protocol/commands/event.h"

#include "../os/Atomic.h"
#include "../os/Thread.h"

#include "../il2cpp-api.h"
#include "../class-internals.h"
#include "../il2cpp-api-types.h"
#include <algorithm>

using namespace il2cpp::debugger;

bool is_debugger_thread()
{
    return Agent::instance().IsDebuggerThread();
}

AgentConfig AgentConfig::Default;

const char* default_host = "localhost";
int32_t default_port = 9876;

AgentConfig::AgentConfig() :
    server(true),
    host(default_host),
    port(default_port)
{
}

AgentConfig::AgentConfig(const char *host, int32_t port) :
    server(true),
    host(host),
    port(port)
{
}

Agent Agent::_instance;

#if IL2CPP_UNIT_TESTS_ENABLED
Il2CppApiTestForwarder Agent::TestForwarder;
#endif // IL2CPP_UNIT_TESTS_ENABLED

Agent::Agent() :
    _thread(0),
    _managed_debugger_thread(0),
    _event_req_list(*this, true),
    _transport(0),
    _is_notifying_pending_events(false),
    _ss_data(0),
    _notify_pending_events_on_first_invoke(false),
    _is_initialized(false),
    _currently_invoking_method(false)
{
}

Agent::~Agent()
{
    _managed_debugger_thread = 0;

    if (_thread != 0)
    {
        delete _thread;
        _thread = 0;
    }

    if (_transport != 0)
    {
        delete _transport;
        _transport = 0;
    }
}

void Agent::Initialize(const AgentConfig &config)
{
    std::stringstream message;
    message << "Listening on " << config.host << ":" << config.port;
    LOG(message.str());

    _config = config;

    _transport = new TCPDataTransport(_config.host, _config.port);
    _transport->Setup();

    // The only implemented mode right now is 'server'
    // if(config.server)
    _transport->WaitForClient();

    // Protocol should take care of buffering the events until a client is connected
    _protocol.Setup(_transport);

    if (!_protocol.Handshake())
    {
        LOG("handshake was not successfull");
        return; // false
    }

    _is_initialized = true;
}

bool Agent::IsInitialized()
{
    return _is_initialized;
}

void Agent::InvalidateBreakpointData()
{
    std::list<BreakpointData*>::iterator it = _bp_data.begin();
    while (it != _bp_data.end())
    {
        INVALIDATE_BREAKPOINT_DATA(*it);
        ++it;
    }
}

void Agent::ClearBreakpointData()
{
    std::list<BreakpointData*>::iterator it = _bp_data.begin();
    while (it != _bp_data.end())
    {
        delete *it;
        ++it;
    }

    _bp_data.clear();
}

void Agent::ClearSingleStepData()
{
    if (_ss_data != 0)
    {
        delete _ss_data;
        _ss_data = 0;
    }
}

SingleStepData *Agent::PrepareSingleStepData(EventRequest *request, Il2CppThread *thread, StepSizeKind size, StepDepthKind depth)
{
    SingleStepData *data = new SingleStepData();
    data->size = size;
    data->depth = depth;
    data->thread = thread;
    data->request = request;
    data->last_line = -1;
    data->last_method = 0;
    data->stack_depth = -1;

    IL2CPP_ASSERT(size == kStepSizeKindLine && "Only kStepSizeKindLine is supported for single stepping for now.");

    Il2CppStackFrameInfo top_frame;

    if (!il2cpp_thread_get_top_frame(thread, top_frame))
        return data;

    if (size == kStepSizeKindLine)
    {
        data->last_method = top_frame.method;
        data->stack_depth = il2cpp_thread_get_stack_depth(thread);

        const Il2CppDebugMethodInfo *debug_method = top_frame.method->debug_info;

        if (debug_method != 0)
        {
            const int32_t *offset_table = il2cpp_debug_method_get_offset_table(debug_method);

            // An offset_table might be empty. In case it is, we skip the lineno.
            // Same thing for the il_offset.
            if (offset_table[0] != -1 && top_frame.il_offset != -1)
            {
                int32_t i;
                const int32_t il_offset = top_frame.il_offset;

                for (i = 2; offset_table[i] >= 0; i += 2)
                {
                    const int32_t start = offset_table[i - 2];
                    const int32_t end = offset_table[i];

                    if (start <= il_offset && end > il_offset)
                    {
                        data->last_line = offset_table[i - 1];
                        break;
                    }
                }

                if (data->last_line == -1)
                {
                    const int32_t last_offset = offset_table[i - 2];
                    if (il_offset <= debug_method->code_size && il_offset >= last_offset)
                        data->last_line = offset_table[i - 1];
                }
            }
        }
    }

    if (depth == kStepDepthKindOver)
    {
        if (top_frame.il_offset != -1)
        {
            //  if (ss_req->depth == STEP_DEPTH_OVER) {
            //  frame_index = 1;
            //  /*
            //   * Find the first sequence point in the current or in a previous frame which
            //   * is not the last in its method.
            //   */
            //  while (sp && sp->next_len == 0) {
            //      sp = NULL;
            //      if (tls && frame_index < tls->frame_count) {
            //          StackFrame *frame = tls->frames [frame_index];

            //          method = frame->method;
            //          if (frame->il_offset != -1) {
            //              sp = find_seq_point (frame->domain, frame->method, frame->il_offset, &info);
            //          }
            //          frame_index ++;
            //      }
            //  }

            //  if (sp && sp->next_len > 0) {
            //      use_bp = TRUE;
            //      for (i = 0; i < sp->next_len; ++i) {
            //          next_sp = &info->seq_points [sp->next [i]];

            //          bp = set_breakpoint (method, next_sp->il_offset, ss_req->req);
            //          ss_req->bps = g_slist_append (ss_req->bps, bp);
            //      }
            //  }
            //}
        }
    }

    //if (ss_req->depth == STEP_DEPTH_OVER) {
    //  StackFrame *frame;

    //  compute_frame_info (thread, tls);

    //  IL2CPP_ASSERT(tls->frame_count);
    //  frame = tls->frames [0];

    //  if (frame->il_offset != -1) {
    //      /* FIXME: Sort the table and use a binary search */
    //      sp = find_seq_point (frame->domain, frame->method, frame->il_offset, &info);
    //      if (!sp) return ERR_NOT_IMPLEMENTED; // This can happen with exceptions when stepping
    //      method = frame->method;
    //  }
    //}

    return data;
}

void Agent::Dispose()
{
    LOG("Disposing debugger agent");

    StopListeningThread();

    _protocol.Dispose();

    delete _transport;
    _transport = 0;

    _event_req_list.ClearAllRequests();

    ClearSingleStepData();
    ClearBreakpointData();

    _is_initialized = false;
}

bool Agent::IsDebuggerThread()
{
    if (_thread == 0)
        return false;

    return _thread->Id() == os::Thread::CurrentThreadId();
}

void Agent::NotifyVmStart()
{
    LOG("Notifying VM Start");

    VmStartEvent vm_start(il2cpp_thread_current());
    NotifyEvent(vm_start, _event_req_list);

    // NotifyAllPendingEvents();

    LOG("VM Start notified");
}

void Agent::NotifyVmDeath()
{
    LOG("Notifying VM Death");

    VmDeathEvent vm_death(il2cpp_thread_current());
    NotifyEvent(vm_death, _event_req_list);

    LOG("VM Death notified");

    StopListeningThread();
}

void Agent::NotifyUserBreak()
{
    LOG("Notifying User Break");

    UserBreakEvent user_break(il2cpp_thread_current());
    NotifyEvent(user_break, _event_req_list);

    LOG("User Break notified");
}

void Agent::NotifyBreakPointHit(const MethodInfo *method, uint64_t offset, BreakpointData &bp_data)
{
    LOG("Notifying Break Point hit");

    EventRequestList request_list(*this, false);

    // TODO: there might be multiple requests matching a given breakpoint in case of multiple
    // threads running the same method. Need to deal with that.
    request_list.AddEventRequest(bp_data.request);

    BreakPointEvent breakpoint_event(il2cpp_thread_current());
    breakpoint_event.method((MethodInfo*)method);
    breakpoint_event.il_offset(offset);
    NotifyEvent(breakpoint_event, request_list);

    LOG("Break Point hit notified");
}

void Agent::NotifyStepHit(const MethodInfo *method, uint64_t offset, SingleStepData &ss_data)
{
    LOG("Notifying Step event");

    EventRequestList request_list(*this, false);

    request_list.AddEventRequest(ss_data.request);

    StepEvent step_event(il2cpp_thread_current());
    step_event.method((MethodInfo*)method);
    step_event.il_offset(offset);
    NotifyEvent(step_event, request_list);

    LOG("Step event notified");
}

void Agent::NotifyAppDomainCreate(Il2CppDomain *domain)
{
    LOG("Notifying AppDomainCreate " << (uintptr_t)domain);

    AppDomainCreateEvent create_event(il2cpp_thread_current());
    create_event.domain(domain);

    NotifyEvent(create_event, _event_req_list);

    LOG("Done notifying AppDomainCreate " << (uintptr_t)domain);
}

void Agent::NotifyAssemblyLoad(const Il2CppAssembly *assembly)
{
    LOG("Notifying AssemblyLoad " << (uintptr_t)assembly);

    AssemblyLoadEvent load_event(il2cpp_thread_current());
    load_event.assembly(const_cast<Il2CppAssembly*>(assembly));

    NotifyEvent(load_event, _event_req_list);

    LOG("Done notifying AssemblyLoad " << (uintptr_t)assembly);
}

void Agent::NotifyTypeLoad(Il2CppClass *type)
{
    LOG("Notifying TypeLoad " << il2cpp_class_get_namespace(type) << "." << il2cpp_class_get_name(type));

    TypeLoadEvent load_event(il2cpp_thread_current());
    load_event.type(type);

    NotifyEvent(load_event, _event_req_list);

    LOG("Done notifying TypeLoad " << il2cpp_class_get_namespace(type) << "." << il2cpp_class_get_name(type));
}

void Agent::NotifyThreadStart(Il2CppThread *thread)
{
    LOG("Notifying ThreadStart " << (uintptr_t)thread);

    ThreadStartEvent thread_start_event(thread);
    NotifyEvent(thread_start_event, _event_req_list);

    LOG("Done notifying ThreadStart " << (uintptr_t)thread);
}

void Agent::NotifyThreadDeath(Il2CppThread *thread)
{
    LOG("Notifying ThreadDeath " << (uintptr_t)thread);

    ThreadDeathEvent thread_death_event(thread);
    NotifyEvent(thread_death_event, _event_req_list);

    LOG("Done notifying ThreadDeath " << (uintptr_t)thread);
}

void Agent::OnEventSkipped(const Event &evt)
{
    if (_is_notifying_pending_events)
        return;

    // The event has been skipped. Based on the type, let's record it to be sent later
    switch (evt.kind())
    {
        case kEventKindAppDomainCreate:
            _pending_domains.insert(((const AppDomainCreateEvent&)evt).domain());
            break;

        case kEventKindAssemblyLoad:
            _pending_assemblies.insert(((const AssemblyLoadEvent&)evt).assembly());
            break;

        case kEventKindTypeLoad:
            _pending_types.insert(((const TypeLoadEvent&)evt).type());
            break;

        case kEventKindAppDomainUnload:
            LOG("OnEventSkipped: kEventKindAppDomainUnload not implemented");
            break;

        case kEventKindAssemblyUnload:
            LOG("OnEventSkipped: kEventKindAssemblyUnload not implemented");
            break;

        case kEventKindThreadStart:
            _pending_threads.insert(evt.thread());
            break;

        case kEventKindThreadDeath:
        {
            // If a thread dies while events are still pending, we can get rid of it
            std::set<Il2CppThread*>::const_iterator it = _pending_threads.find(evt.thread());
            if (it != _pending_threads.end())
                _pending_threads.erase(it);
        }
        break;

        case kEventKindVmStart:
        case kEventKindVmDeath:
        case kEventKindMethodEntry:
        case kEventKindMethodExit:
        case kEventKindBreakpoint:
        case kEventKindStep:
        case kEventKindException:
        case kEventKindKeepAlive:
        case kEventKindUserBreak:
        case kEventKindUserLog:
            break;
    }
}

void Agent::NotifyAllPendingEvents()
{
    LOG("NotifyAllPendingEvents");

    NotifyAppDomainCreatePendingEvents();
    NotifyThreadStartPendingEvents();
    NotifyAssemblyLoadPendingEvents();
    NotifyTypeLoadPendingEvents();
}

void Agent::NotifyAppDomainCreatePendingEvents()
{
    LOG("Notifying pending AppDomainCreate events");

    _is_notifying_pending_events = true;

    std::set<Il2CppDomain*>::const_iterator dit = _pending_domains.begin();

    while (dit != _pending_domains.end())
    {
        NotifyAppDomainCreate(*dit);
        ++dit;
    }

    _pending_domains.clear();

    _is_notifying_pending_events = false;

    LOG("Done notifying pending AppDomainCreate events");
}

void Agent::NotifyAssemblyLoadPendingEvents()
{
    LOG("Notifying pending AssemblyLoad events");

    _is_notifying_pending_events = true;

    std::set<Il2CppAssembly*>::const_iterator dit = _pending_assemblies.begin();

    while (dit != _pending_assemblies.end())
    {
        NotifyAssemblyLoad(*dit);
        ++dit;
    }

    _pending_assemblies.clear();

    _is_notifying_pending_events = false;

    LOG("Done notifying pending AssemblyLoad events");
}

void Agent::NotifyTypeLoadPendingEvents()
{
    LOG("Notifying pending TypeLoad events");

    _is_notifying_pending_events = true;

    std::set<Il2CppClass*>::const_iterator dit = _pending_types.begin();

    while (dit != _pending_types.end())
    {
        NotifyTypeLoad(*dit);
        ++dit;
    }

    _pending_types.clear();

    _is_notifying_pending_events = false;

    LOG("Done notifying pending TypeLoad events");
}

void Agent::NotifyThreadStartPendingEvents()
{
    LOG("Notifying pending ThreadStart events");

    _is_notifying_pending_events = true;

    std::set<Il2CppThread*>::const_iterator dit = _pending_threads.begin();

    while (dit != _pending_threads.end())
    {
        NotifyThreadStart(*dit);
        ++dit;
    }

    _pending_threads.clear();

    _is_notifying_pending_events = false;

    LOG("Done notifying pending ThreadStart events");
}

void Agent::NotifyPendingEventsOfKind(EventKind kind)
{
    switch (kind)
    {
        case kEventKindAppDomainCreate:
            NotifyAppDomainCreatePendingEvents();
            break;

        case kEventKindAssemblyLoad:
            NotifyAssemblyLoadPendingEvents();
            break;

        case kEventKindTypeLoad:
            NotifyTypeLoadPendingEvents();
            break;

        case kEventKindAppDomainUnload:
            LOG("NotifyPendingEventsOfKind: kEventKindAppDomainUnload not implemented");
            break;

        case kEventKindAssemblyUnload:
            LOG("NotifyPendingEventsOfKind: kEventKindAssemblyUnload not implemented");
            break;

        case kEventKindThreadStart:
            NotifyThreadStartPendingEvents();
            break;

        case kEventKindThreadDeath:
        case kEventKindVmStart:
        case kEventKindVmDeath:
        case kEventKindMethodEntry:
        case kEventKindMethodExit:
        case kEventKindBreakpoint:
        case kEventKindStep:
        case kEventKindException:
        case kEventKindKeepAlive:
        case kEventKindUserBreak:
        case kEventKindUserLog:
            break;
    }
}

bool Agent::ShouldSendPendingEventsOnFirstMethodEntry()
{
    return _notify_pending_events_on_first_invoke;
}

void Agent::NotifyEvent(const Event &evt, EventRequestList &event_reqs)
{
    if (!_event_req_list.IsClientInterested(evt))
    {
        LOG("Skipping event, client not interested!");
        OnEventSkipped(evt);
        return;
    }

    IL2CPP_ASSERT(evt.thread() != 0 && "A debugger event is being sent with a NULL thread_id, which is not allowed (aka: something very bad happened).");

    EventRequestList::FilterInfo filter_info;

    filter_info.exception = 0;
    filter_info.klass = 0;

    std::vector<int32_t> ids;
    event_reqs.RequestIdsFor(ids, evt, filter_info);

    SuspendPolicy suspend_policy = kSuspendPolicyNone;

    std::vector<int32_t>::iterator it = ids.begin();
    while (it != ids.end())
    {
        const EventRequest *req = event_reqs.GetEventRequestById(*it);
        if (req != NULL)
            suspend_policy = std::max(suspend_policy, req->suspend_policy());
        ++it;
    }

    EventCompositeCommand command;

    it = ids.begin();
    while (it != ids.end())
    {
        command.AddEvent(*it, &evt);
        ++it;
    }

    if (evt.kind() == kEventKindVmStart)
    {
        IL2CPP_ASSERT(!IsDebuggerThread() && "Unexpected VmStart event sent from the debugger thread. Only VM threads are allowed to send it.");

        // TODO: move this somewhere else, or split the NotifyEvent logic if needed

        // (suspend=y)
        suspend_policy = kSuspendPolicyAll;
        StartListeningThread();
    }
    else
    {
        if (IsDebuggerThread())
        {
            LOG("The event is being sent from the debugger thread. Disabling suspend.");

            suspend_policy = kSuspendPolicyNone;
        }
    }

    // We always suspend in case a Step Event is fired
    if (evt.kind() == kEventKindStep)
        suspend_policy = kSuspendPolicyAll;

    if (suspend_policy == kSuspendPolicyAll)
        RequestSuspend();

    command.suspend_policy(suspend_policy);

    _protocol.SendCommand(&command);

    if (!IsDebuggerThread())
        SuspendCurrentThreadIfNeeded();
}

void Agent::OnMethodEntry(const Il2CppStackFrameInfo &info)
{
    if (ShouldSendPendingEventsOnFirstMethodEntry())
    {
        NotifyAllPendingEvents();

        _notify_pending_events_on_first_invoke = false;
    }
}

void Agent::OnMethodExit(const Il2CppStackFrameInfo &info)
{
}

void Agent::OnThreadAttach(Il2CppThread *thread)
{
    if (IsDebuggerThread())
        return;

    _thread_data.NotifyThreadAttach(thread);

    NotifyThreadStart(thread);

    SuspendCurrentThreadIfNeeded();
}

void Agent::OnThreadDetach(Il2CppThread *thread)
{
    if (IsDebuggerThread())
        return;

    NotifyThreadDeath(thread);

    _thread_data.NotifyThreadDetach(thread);
}

bool Agent::IsSuspended()
{
    return il2cpp::os::Atomic::CompareExchange(&suspend_count, suspend_count, 0) != 0;
}

void Agent::RequestSuspend()
{
    LOG("Requesting VM suspend");

    il2cpp::os::Atomic::Increment(&suspend_count);
}

void Agent::WaitForSuspend()
{
    LOG("Waiting for all the managed threads to be suspended.");

    LOG("Agent::WaitForSuspend has a dummy implementation for now.");

    suspend_semaphore.Wait();

    LOG("VM suspended");
}

void Agent::RequestResume()
{
    LOG("Requesting VM resume");

    il2cpp::os::Atomic::Decrement(&suspend_count);

    IL2CPP_ASSERT(suspend_count >= 0 && "RequestResume has been invoked more times than RequestSuspend!");
}

void Agent::SuspendCurrentThreadIfNeeded()
{
    IL2CPP_ASSERT(!IsDebuggerThread() && "The debugger thread cannot be suspended!");

    if (il2cpp::os::Atomic::CompareExchange(&suspend_count, suspend_count, 0) == 0)
        return;

    LOG("Suspending current thread");

    suspend_semaphore.Post(1, NULL);

    ThreadData* current_thread_data = _thread_data.ThreadDataFor(CALL_IL2CPP_API(il2cpp_thread_current)());

    while (il2cpp::os::Atomic::CompareExchange(&suspend_count, suspend_count, 0) != 0)
    {
        il2cpp::os::Thread::Sleep(1);
        InvokeMethodIfOneIsReady(current_thread_data);
    }

    current_thread_data->InvalidateFramesCache();

    LOG("Resuming current thread");
}

void Agent::SequencePointHit(int64_t uid, int32_t offset)
{
    // Note: SequencePointHit locks in case a breakpoint or a single step is hit, and returns only when the VM is resumed.

    // LOG("Sequence point hit (uid: " << uid << ", offset: " << std::uppercase << std::hex << offset << std::dec << std::nouppercase << ")");

    if (_currently_invoking_method)
        return;

    il2cpp_debug_update_frame_il_offset(offset);

    SuspendCurrentThreadIfNeeded();

    BreakpointData *bp_data = BreakpointDataAt(uid, offset);

    if (IS_BREAKPOINT_DATA_INVALID(bp_data))
    {
        LOG("Breakpoint hit (request id: " << bp_data->request->id() << ")");

        ClearSingleStepData();

        NotifyBreakPointHit(bp_data->method, offset, *bp_data);
    }
    else
    {
        // TODO: we probably need to do something different in case there
        // were both a bp and a single step at the same line.
        ProcessSingleStepIfNeeded(uid, offset);
    }
}

BreakpointData *Agent::BreakpointDataAt(int64_t uid, int32_t offset)
{
    Il2CppStackFrameInfo frame;

    if (!il2cpp_current_thread_get_top_frame(frame))
    {
        IL2CPP_ASSERT(0 && "Not implemented");
    }

    const Il2CppDebugMethodInfo *info = il2cpp_debug_get_method_info(frame.method);

    return (BreakpointData*)il2cpp_debug_method_get_breakpoint_data_at(info, uid, offset);
}

bool Agent::ShouldProcessStepRequest(Il2CppThread *current_thread, const Il2CppStackFrameInfo *top_frame, int32_t stack_depth, bool *can_break_at_the_same_line)
{
    if (_ss_data == 0)
        return false;

    *can_break_at_the_same_line = true;

    switch (_ss_data->depth)
    {
        case kStepDepthKindInto:
        {
            if (_ss_data->last_method != top_frame->method)
            {
                Il2CppStackFrameInfo second_top_frame;
                if (!il2cpp_thread_get_frame_at(current_thread, -1, second_top_frame))
                    return false;

                if (_ss_data->last_method != second_top_frame.method)
                    return false;
            }
            else
            {
                // In this case, we perform a simple step over.
                return true;
            }
        }
        break;

        case kStepDepthKindOver:
            *can_break_at_the_same_line = false;

            if (_ss_data->last_method != top_frame->method)
            {
                if (stack_depth < _ss_data->stack_depth)
                {
                    // We stepped over the last sequence point of a function invocation.
                    // In this case, we perform a step out.
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                if (stack_depth == _ss_data->stack_depth)
                {
                    // Regular step over.
                    return true;
                }
                else if (stack_depth < _ss_data->stack_depth)
                {
                    // We stepped over the last sequence point of a recursive function invocation.
                    // In this case, we perform a step out.
                    // In this case, we might also be at the last instruction of a recursive function: this
                    // means that, after the simulation of the step out, we end up on the same line we were before.
                    // In this case, we should still allow the agent to break.
                    *can_break_at_the_same_line = true;
                    return true;
                }
                else
                {
                    // We are inside a recursive function. In this case we need to skip the step.
                    return false;
                }
            }

            break;

        case kStepDepthKindOut:
            if (_ss_data->last_method == top_frame->method)
            {
                if (stack_depth >= _ss_data->stack_depth)
                    return false;

                // In this case, we are probably stepping out of a recursive function. In this case
                // we can proceed simulating a step over.
                return true;
            }

            if (stack_depth >= _ss_data->stack_depth)
                return false;

            IL2CPP_ASSERT((_ss_data->stack_depth - stack_depth) == 1 && "Stepping out jumped more than one stack frame: not supported yet (probably hitting a native stack frame)");
            break;
    }

    return true;
}

bool Agent::ProcessSingleStepIfNeeded(int64_t uid, int32_t offset)
{
    if (_ss_data == 0)
        return false;

    Il2CppThread *current_thread = il2cpp_thread_current();

    if (_ss_data->thread != current_thread)
        return false;

    IL2CPP_ASSERT(_ss_data->size == kStepSizeKindLine && "Only line stepping is supported for now");

    Il2CppStackFrameInfo top_frame;

    if (!il2cpp_current_thread_get_top_frame(top_frame))
        return false;

    if (top_frame.il_offset == -1)
        return false;

    bool can_break_at_the_same_line = true;
    const int32_t stack_depth = il2cpp_current_thread_get_stack_depth();

    if (!ShouldProcessStepRequest(current_thread, &top_frame, stack_depth, &can_break_at_the_same_line))
        return false;

    const Il2CppDebugMethodInfo *debug_method = top_frame.method->debug_info;

    if (debug_method == 0)
        return false;

    _ss_data->stack_depth = stack_depth;
    _ss_data->last_method = top_frame.method;

    int32_t lineno = -1;
    const int32_t *offset_table = il2cpp_debug_method_get_offset_table(debug_method);

    // An offset_table might be empty. In case it is, we skip the lineno.
    // Same thing for the il_offset.
    if (offset_table[0] != -1 && top_frame.il_offset != -1)
    {
        int32_t i;
        const int32_t il_offset = top_frame.il_offset;

        for (i = 2; offset_table[i] >= 0; i += 2)
        {
            const int32_t start = offset_table[i - 2];
            const int32_t end = offset_table[i];

            if (start <= il_offset && end > il_offset)
            {
                lineno = offset_table[i - 1];
                break;
            }
        }

        if (lineno == -1)
        {
            const int32_t last_offset = offset_table[i - 2];
            if (il_offset <= debug_method->code_size && il_offset >= last_offset)
                lineno = offset_table[i - 1];
        }
    }

    if (lineno == -1 || (lineno == _ss_data->last_line && !can_break_at_the_same_line))
        return false;

    SingleStepData ss_data_copy = *_ss_data;

    ss_data_copy.last_line = lineno;

    ClearSingleStepData();

    NotifyStepHit(ss_data_copy.last_method, offset, ss_data_copy);

    return true;
}

static void start_debugger_agent_thread(void *data)
{
    Agent *agent = (Agent*)data;
    agent->ListenForClientMessages();
}

void Agent::StartListeningThread()
{
    IL2CPP_ASSERT(_thread == 0 && "Listening thread is already running!");

    _thread = new os::Thread();
    _thread->Run(start_debugger_agent_thread, this);
}

void Agent::StopListeningThread()
{
    if (_thread == 0)
        return;

    _protocol.Dispose();

    _thread->Join();

    delete _thread;
    _thread = 0;
}

#define DELETE_LOCAL(v) \
    if(v != 0) \
        delete v; \
    v = 0;

void Agent::ListenForClientMessages()
{
    LOG("Listening for incoming messages.");

    // Attach the thread where the debugger is running so that the GC will work
    // correctly with managed objects used in the debugger thread.
    _managed_debugger_thread = il2cpp_thread_attach(il2cpp_domain_get());

    while (true)
    {
        const Command *command = _protocol.WaitForCommand();

        if (command == NULL)
        {
            LOG("The socket was shut down while reading the header");
            break;
        }

        const Reply *reply = Process(command);

        if (reply != NULL)
        {
            if (!_protocol.SendReply(reply))
            {
                DELETE_LOCAL(reply);
                DELETE_LOCAL(command);

                LOG("The socket was shut down while sending the reply");
                break;
            }
        }

        if (command->is(kCommandSetVm, kCommandSetVmDispose))
        {
            DELETE_LOCAL(reply);
            DELETE_LOCAL(command);
            break;
        }

        DELETE_LOCAL(reply);
        DELETE_LOCAL(command);
    }

    LOG("Debugger thread is being stopped");

    _protocol.Dispose();

    il2cpp_thread_detach(il2cpp_thread_current());
}

void Agent::InvokeMethodIfOneIsReady(ThreadData* thread_data)
{
    const MethodInfo* method = thread_data->GetMethodToInvoke();
    if (method)
    {
        CurrentlyInvokingMethodHelper helper(_currently_invoking_method);
        thread_data->SetReturnValueOfMethodToInvoke(CALL_IL2CPP_API(il2cpp_runtime_invoke)(method, NULL, NULL, NULL));
        thread_data->ClearMethodToInvoke();
        thread_data->SignalMethodInvokeComplete();
    }
}

#define PROCESS_COMMAND(s, c) \
    if(command->is(kCommandSet ## s, kCommandSet ## s ## c)) \
        return Process((const s ## c ## Command*)command)

const Reply *Agent::Process(const Command *command)
{
    PROCESS_COMMAND(StackFrame, GetValues);
    PROCESS_COMMAND(StackFrame, SetValues);
    PROCESS_COMMAND(StackFrame, GetThis);
    PROCESS_COMMAND(Assembly, GetObject);
    PROCESS_COMMAND(Assembly, GetEntryPoint);
    PROCESS_COMMAND(Assembly, GetManifestModule);
    PROCESS_COMMAND(Assembly, GetName);
    PROCESS_COMMAND(Assembly, GetLocation);
    PROCESS_COMMAND(Assembly, GetType);
    PROCESS_COMMAND(InternalError, NotImplemented);
    PROCESS_COMMAND(Vm, SetProtocolVersion);
    PROCESS_COMMAND(Vm, Suspend);
    PROCESS_COMMAND(Vm, AllThreads);
    PROCESS_COMMAND(Vm, Version);
    PROCESS_COMMAND(Vm, Dispose);
    PROCESS_COMMAND(Vm, AbortInvoke);
    PROCESS_COMMAND(Vm, Resume);
    PROCESS_COMMAND(Vm, Exit);
    PROCESS_COMMAND(Vm, InvokeMethod);
    PROCESS_COMMAND(StringRef, GetValue);
    PROCESS_COMMAND(ObjectRef, GetType);
    PROCESS_COMMAND(ObjectRef, GetDomain);
    PROCESS_COMMAND(ObjectRef, IsCollected);
    PROCESS_COMMAND(ObjectRef, SetValues);
    PROCESS_COMMAND(ObjectRef, GetAddress);
    PROCESS_COMMAND(ObjectRef, GetValues);
    PROCESS_COMMAND(Type, GetProperties);
    PROCESS_COMMAND(Type, GetSourceFiles2);
    PROCESS_COMMAND(Type, GetPropertyCAttrs);
    PROCESS_COMMAND(Type, GetFieldCAttrs);
    PROCESS_COMMAND(Type, GetInfo);
    PROCESS_COMMAND(Type, IsAssignableFrom);
    PROCESS_COMMAND(Type, GetCAttrs);
    PROCESS_COMMAND(Type, GetSourceFiles);
    PROCESS_COMMAND(Type, GetValues);
    PROCESS_COMMAND(Type, GetObject);
    PROCESS_COMMAND(Type, GetFields);
    PROCESS_COMMAND(Type, SetValues);
    PROCESS_COMMAND(Type, GetMethods);
    PROCESS_COMMAND(Thread, GetName);
    PROCESS_COMMAND(Thread, GetFrameInfo);
    PROCESS_COMMAND(Thread, GetInfo);
    PROCESS_COMMAND(Thread, GetState);
    PROCESS_COMMAND(Thread, GetId);
    PROCESS_COMMAND(ArrayRef, GetValues);
    PROCESS_COMMAND(ArrayRef, GetLength);
    PROCESS_COMMAND(ArrayRef, SetValues);
    PROCESS_COMMAND(EventRequest, Clear);
    PROCESS_COMMAND(EventRequest, ClearAllBreakpoints);
    PROCESS_COMMAND(EventRequest, Set);
    PROCESS_COMMAND(AppDomain, GetFriendlyName);
    PROCESS_COMMAND(AppDomain, CreateString);
    PROCESS_COMMAND(AppDomain, GetEntryAssembly);
    PROCESS_COMMAND(AppDomain, GetRootDomain);
    PROCESS_COMMAND(AppDomain, CreateBoxedValue);
    PROCESS_COMMAND(AppDomain, GetAssemblies);
    PROCESS_COMMAND(AppDomain, GetCorlib);
    PROCESS_COMMAND(Method, GetParamInfo);
    PROCESS_COMMAND(Method, GetName);
    PROCESS_COMMAND(Method, GetDeclaringType);
    PROCESS_COMMAND(Method, GetLocalsInfo);
    PROCESS_COMMAND(Method, GetInfo);
    PROCESS_COMMAND(Method, GetDebugInfo);
    PROCESS_COMMAND(Method, ResolveToken);
    PROCESS_COMMAND(Method, GetBody);
    PROCESS_COMMAND(Module, GetInfo);
    PROCESS_COMMAND(Event, Composite);

    return NULL;
}

const Protocol& Agent::GetProtocol()
{
    return _protocol;
}

#endif
