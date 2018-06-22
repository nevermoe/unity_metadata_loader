#if IL2CPP_DEBUGGER_ENABLED

#pragma once

#include "../../il2cpp-api.h"
#include "../../metadata.h"

#include "../agent.h"
#include "../protocol/commands/vm.h"
#include "../protocol/commands/internal-error.h"

namespace il2cpp
{
namespace debugger
{
    const Reply *Agent::Process(const VmVersionCommand *command)
    {
        VmVersionCommand::Reply *vm_version_reply = command->reply();

        vm_version_reply->name("il2cpp 0.0.1");
        vm_version_reply->major_version(_protocol.major_version());
        vm_version_reply->minor_version(_protocol.minor_version());

        return vm_version_reply;
    }

    const Reply *Agent::Process(const VmSetProtocolVersionCommand *command)
    {
        _protocol.major_version(command->major_version());
        _protocol.minor_version(command->minor_version());

        LOG("Protocol version " << Protocol::MajorVersion << "." << Protocol::MinorVersion << ", client protocol version " << _protocol.major_version() << "." << _protocol.minor_version() << ".");

        return command->reply();
    }

    const Reply *Agent::Process(const VmResumeCommand *command)
    {
        VmResumeCommand::Reply *empty_reply = command->reply();

        if (!IsSuspended())
            empty_reply->error_code(kErrorCodeNotSuspended);
        else
            RequestResume();

        return empty_reply;
    }

    const Reply *Agent::Process(const VmAllThreadsCommand *command)
    {
        VmAllThreadsCommand::Reply *all_threads_reply = command->reply();

        size_t size = 0;
        Il2CppThread **threads = CALL_IL2CPP_API(il2cpp_thread_get_all_attached_threads) (&size);

        for (size_t i = 0; i < size; ++i)
        {
            Il2CppThread *thread = threads[i];
            if (!CALL_IL2CPP_API(il2cpp_is_vm_thread)(thread))
                continue;

            if (thread == _managed_debugger_thread)
                continue;

            all_threads_reply->threads().push_back(thread);
        }

        return all_threads_reply;
    }

    const Reply *Agent::Process(const VmDisposeCommand *command)
    {
        VmDisposeCommand::Reply *empty_reply = command->reply();

        LOG("warning: probably we should use a global lock here to avoid contention on event_req_list.");

        InvalidateBreakpointData();

        _event_req_list.ClearAllRequests();

        while (IsSuspended())
            RequestResume();

        return empty_reply;
    }

    const Reply *Agent::Process(const VmSuspendCommand *command)
    {
        VmSuspendCommand::Reply *empty_reply = command->reply();

        if (!IsSuspended())
        {
            RequestSuspend();
            WaitForSuspend();
        }

        return empty_reply;
    }

    const Reply *Agent::Process(const VmAbortInvokeCommand *command)
    {
        LOG("warning: `VmAbortInvokeCommand` not implemented. Returning a `NotImplemented` reply!");

        IL2CPP_ASSERT(0);

        return new InternalErrorNotImplementedReply(command);
    }

    const Reply *Agent::Process(const VmExitCommand *command)
    {
        LOG("warning: `VmExitCommand` not implemented. Returning a `NotImplemented` reply!");

        IL2CPP_ASSERT(0);

        return new InternalErrorNotImplementedReply(command);
    }

    const Reply *Agent::Process(const VmInvokeMethodCommand *command)
    {
        VmInvokeMethodCommand::Reply *reply = command->reply();

        if (!IsSuspended())
        {
            reply->error_code(kErrorCodeNotSuspended);
            return reply;
        }

        Il2CppThread *vm_thread = command->vm_thread();
        int32_t flags = command->flags();
        const MethodInfo* method = command->method();

        ThreadData* vm_thread_data = _thread_data.ThreadDataFor(vm_thread);

        vm_thread_data->SetMethodToInvoke(method);

        vm_thread_data->WaitForMethodToBeInvoked();

        reply->method_did_not_throw_exception(true);

        const Il2CppType* return_type = CALL_IL2CPP_API(il2cpp_method_get_return_type)(method);
        reply->return_type(return_type->type);

        if (return_type->type == IL2CPP_TYPE_VOID)
        {
            reply->skip_return_value = true;
        }
        else
        {
            int32_t actual_return = *(int32_t*)(CALL_IL2CPP_API(il2cpp_object_unbox)(vm_thread_data->GetReturnValueOfMethodToInvoke()));
            reply->return_value(actual_return);
        }

        return reply;
    }
}
}

#endif
