#if IL2CPP_DEBUGGER_ENABLED

#pragma once

#include "../../il2cpp-api.h"

#include "../agent.h"
#include "../protocol/commands/thread.h"
#include "../protocol/commands/internal-error.h"

namespace il2cpp
{
namespace debugger
{
    const Reply *Agent::Process(const ThreadGetIdCommand *command)
    {
        ThreadGetIdCommand::Reply *get_id_reply = command->reply();

        get_id_reply->id((uintptr_t)command->thread());

        return get_id_reply;
    }

    const Reply *Agent::Process(const ThreadGetNameCommand *command)
    {
        ThreadGetNameCommand::Reply *get_name_reply = command->reply();

        uint32_t len = 0;
        char *name = il2cpp_thread_get_name(command->thread(), &len);

        get_name_reply->name(std::string(name, len));

        if (name)
            delete name;

        return get_name_reply;
    }

    const Reply *Agent::Process(const ThreadGetInfoCommand *command)
    {
        ThreadGetInfoCommand::Reply *get_info_reply = command->reply();

        LOG("warning: `ThreadGetInfoCommand` is not implemented properly");

        // TODO: assuming thread is never in a pool for now
        get_info_reply->is_thread_pool(false);

        return get_info_reply;
    }

    const Reply *Agent::Process(const ThreadGetFrameInfoCommand *command)
    {
        ThreadGetFrameInfoCommand::Reply *get_frame_info_reply = command->reply();

        // TODO: assert that the VM is suspended or suspending; in the second case, wait for the suspension to
        // complete.
        // if(IsSuspending()) WaitForSuspend();

        IL2CPP_ASSERT(IsSuspended() && "`ThreadGetFrameInfoCommand` can only be invoked with the VM suspended or suspending.");

        ThreadData *data = _thread_data.ThreadDataFor(command->thread());
        data->UpdateFramesCacheIfNeeded();

        std::vector<const Il2CppStackFrameInfo*>::const_iterator it = data->frames_cache().begin();

        while (it != data->frames_cache().end())
        {
            const Il2CppStackFrameInfo *frame = *it;
            ThreadGetFrameInfoCommand::Reply::FrameInfo frame_info;

            frame_info.flags = 0;
            frame_info.frame_id = frame->id;
            frame_info.il_offset = frame->il_offset;
            frame_info.method = frame->method;

            get_frame_info_reply->infos().push_back(frame_info);

            ++it;
        }

        return get_frame_info_reply;
    }

    const Reply *Agent::Process(const ThreadGetStateCommand *command)
    {
        LOG("warning: `ThreadGetStateCommand` not implemented. Returning a `NotImplemented` reply!");

        IL2CPP_ASSERT(0);

        return new InternalErrorNotImplementedReply(command);
    }
}
}

#endif
