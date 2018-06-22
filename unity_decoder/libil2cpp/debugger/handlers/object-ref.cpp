#if IL2CPP_DEBUGGER_ENABLED

#pragma once

#include "../../il2cpp-api.h"

#include "../agent.h"
#include "../protocol/commands/object-ref.h"
#include "../protocol/commands/internal-error.h"

using namespace il2cpp::debugger;

const Reply *Agent::Process(const ObjectRefGetTypeCommand *command)
{
    ObjectRefGetTypeCommand::Reply *get_type_reply = command->reply();

    Il2CppObject *object = command->object();

    get_type_reply->type(il2cpp_object_get_class(object));

    return get_type_reply;
}

const Reply *Agent::Process(const ObjectRefGetDomainCommand *command)
{
    ObjectRefGetDomainCommand::Reply *get_domain_reply = command->reply();

    Il2CppObject *object = command->object();

    // TODO: use `object` even if we don't really have domains in il2cpp?

    get_domain_reply->domain(il2cpp_domain_get());

    return get_domain_reply;
}

const Reply *Agent::Process(const ObjectRefIsCollectedCommand *command)
{
    LOG("warning: `ObjectRefIsCollectedCommand` not implemented. Returning a `NotImplemented` reply!");

    IL2CPP_ASSERT(0);

    return new InternalErrorNotImplementedReply(command);
}

const Reply *Agent::Process(const ObjectRefSetValuesCommand *command)
{
    LOG("warning: `ObjectRefSetValuesCommand` not implemented. Returning a `NotImplemented` reply!");

    IL2CPP_ASSERT(0);

    return new InternalErrorNotImplementedReply(command);
}

const Reply *Agent::Process(const ObjectRefGetAddressCommand *command)
{
    LOG("warning: `ObjectRefGetAddressCommand` not implemented. Returning a `NotImplemented` reply!");

    IL2CPP_ASSERT(0);

    return new InternalErrorNotImplementedReply(command);
}

const Reply *Agent::Process(const ObjectRefGetValuesCommand *command)
{
    LOG("warning: `ObjectRefGetValuesCommand` not implemented. Returning a `NotImplemented` reply!");

    IL2CPP_ASSERT(0);

    return new InternalErrorNotImplementedReply(command);
}

#endif
