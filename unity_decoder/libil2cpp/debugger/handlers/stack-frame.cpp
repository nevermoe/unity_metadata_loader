#if IL2CPP_DEBUGGER_ENABLED

#pragma once

#include <vector>

#include "../../il2cpp-api.h"
#include "../../class-internals.h"

#include "../agent.h"
#include "../protocol/commands/stack-frame.h"
#include "../protocol/commands/internal-error.h"

namespace il2cpp
{
namespace debugger
{
    struct FrameFinderData
    {
        int32_t frame_id;
        int32_t counter;
        const Il2CppStackFrameInfo *info;
    };

    static void find_stack_frame(Il2CppThread* thread, const Il2CppStackFrameInfo *info, void *user_data)
    {
        FrameFinderData *data = (FrameFinderData*)user_data;

        if (++data->counter == data->frame_id)
            data->info = info;
    }

    const Reply *Agent::Process(const StackFrameGetValuesCommand *command)
    {
        ThreadData *data = _thread_data.ThreadDataFor(command->thread());

        IL2CPP_ASSERT(data->IsFramesCacheValid() && "ThreadData frames cache is invalid when invoking StackFrameGetValuesCommand");

        const Il2CppStackFrameInfo *frame = data->FrameById(command->frame_id());

        IL2CPP_ASSERT(frame != NULL && "Couldn't find a stack frame with the given ID.");

        const MethodInfo *method = frame->method;

        StackFrameGetValuesCommand::Reply *get_values_reply = command->reply();

        std::vector<int32_t>::const_iterator it = command->positions().begin();

        while (it != command->positions().end())
        {
            int32_t pos = *it;
            const Il2CppType *type = 0;
            void *addr = 0;

            if (pos < 0)
            {
                pos = -pos - 1;

                IL2CPP_ASSERT(pos < frame->method->parameters_count && "StackFrameGetValuesCommand was invoked with an outside of range param position");

                type = frame->method->parameters[pos].parameter_type;
                addr = frame->params[pos];
            }
            else
            {
                IL2CPP_ASSERT(pos < (int32_t)frame->locals_count && "StackFrameGetValuesCommand was invoked with an outside of range local position");

                type = frame->method->debug_info->locals[pos]->type->byval_arg;
                addr = frame->locals[pos];
            }

            get_values_reply->values().push_back(Variant(type, addr));

            ++it;
        }

        return get_values_reply;
    }

    const Reply *Agent::Process(const StackFrameSetValuesCommand *command)
    {
        ThreadData *data = _thread_data.ThreadDataFor(command->thread());

        IL2CPP_ASSERT(data->IsFramesCacheValid() && "ThreadData frames cache is invalid when invoking StackFrameSetValuesCommand");

        const Il2CppStackFrameInfo *frame = data->FrameById(command->frame_id());

        IL2CPP_ASSERT(frame != NULL && "Couldn't find a stack frame with the given ID.");

        const MethodInfo *method = frame->method;
        const int32_t count = command->values_count();
        Buffer &buffer = command->command_buffer();

        StackFrameSetValuesCommand::Reply *empty_reply = command->reply();

        for (int32_t i = 0; i < count; ++i)
        {
            int32_t pos = buffer.ReadInt();

            const Il2CppType *expected_type = 0;
            void *addr = 0;

            if (pos < 0)
            {
                pos = -pos - 1;

                IL2CPP_ASSERT(pos < frame->method->parameters_count && "StackFrameSetValuesCommand was invoked with an outside of range param position");

                expected_type = frame->method->parameters[pos].parameter_type;
                addr = frame->params[pos];
            }
            else
            {
                IL2CPP_ASSERT(pos < (int32_t)frame->locals_count && "StackFrameSetValuesCommand was invoked with an outside of range local position");

                expected_type = frame->method->debug_info->locals[pos]->type->byval_arg;
                addr = frame->locals[pos];
            }

            Variant::ReadInto(buffer, expected_type, addr);
        }

        return empty_reply;
    }

    const Reply *Agent::Process(const StackFrameGetThisCommand *command)
    {
        ThreadData *data = _thread_data.ThreadDataFor(command->thread());

        IL2CPP_ASSERT(data->IsFramesCacheValid() && "ThreadData frames cache is invalid when invoking StackFrameGetThis");

        const Il2CppStackFrameInfo *frame = data->FrameById(command->frame_id());

        IL2CPP_ASSERT(frame != NULL && "Couldn't find a stack frame with the given ID.");

        const MethodInfo *method = frame->method;

        StackFrameGetThisCommand::Reply *get_this_reply = command->reply();

        get_this_reply->this_object = VTypeVariant(
                method->declaring_type->byval_arg,
                frame->this_ptr);

        return get_this_reply;
    }
}
}

#endif
