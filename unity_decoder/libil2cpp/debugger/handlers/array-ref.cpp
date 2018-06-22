#if IL2CPP_DEBUGGER_ENABLED

#pragma once

#include "../../il2cpp-api.h"
#include "../../class-internals.h"
#include "../../object-internals.h"

#include "../agent.h"
#include "../protocol/commands/array-ref.h"
#include "../protocol/commands/internal-error.h"

namespace il2cpp
{
namespace debugger
{
    const Reply *Agent::Process(const ArrayRefGetValuesCommand *command)
    {
        ArrayRefGetValuesCommand::Reply *get_values_reply = command->reply();

        Il2CppArray *arr = (Il2CppArray*)command->object();
        if (arr == 0)
            return get_values_reply;

        Il2CppClass *array_klass = il2cpp_object_get_class(arr);

        int32_t element_size = il2cpp_array_element_size(array_klass);
        for (int32_t i = command->index(); i < command->index() + command->length(); ++i)
        {
            void *element = (void*)((uint8_t*)arr + kIl2CppSizeOfArray + (i * element_size));
            get_values_reply->values().push_back(
                Variant(array_klass->element_class->byval_arg, element));
        }

        return get_values_reply;
    }

    const Reply *Agent::Process(const ArrayRefGetLengthCommand *command)
    {
        ArrayRefGetLengthCommand::Reply *get_length_reply = command->reply();

        Il2CppArray *arr = (Il2CppArray*)command->object();
        if (arr == 0)
            return get_length_reply;

        Il2CppClass *array_klass = il2cpp_object_get_class(arr);

        get_length_reply->rank(array_klass->rank);

        if (!arr->bounds)
        {
            get_length_reply->lengths().push_back(arr->max_length);
            get_length_reply->lower_bounds().push_back(0);
        }
        else
        {
            for (uint8_t i = 0; i < array_klass->rank; ++i)
            {
                get_length_reply->lengths().push_back(arr->bounds[i].length);
                get_length_reply->lower_bounds().push_back(arr->bounds[i].lower_bound);
            }
        }

        return get_length_reply;
    }

    const Reply *Agent::Process(const ArrayRefSetValuesCommand *command)
    {
        LOG("warning: `ArrayRefSetValuesCommand` not implemented. Returning a `NotImplemented` reply!");

        IL2CPP_ASSERT(0);

        return new InternalErrorNotImplementedReply(command);
    }
}
}

#endif
