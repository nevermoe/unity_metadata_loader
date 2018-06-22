#if IL2CPP_DEBUGGER_ENABLED

#pragma once

#include <algorithm>
#include "../../il2cpp-api.h"

#include "../agent.h"
#include "../protocol/commands/app-domain.h"
#include "../protocol/commands/internal-error.h"

using namespace il2cpp::debugger;

const Reply *Agent::Process(const AppDomainGetRootDomainCommand *command)
{
    AppDomainGetRootDomainCommand::Reply *app_domain_get_root_domain_reply = command->reply();

    app_domain_get_root_domain_reply->domain(CALL_IL2CPP_API(il2cpp_domain_get)());

    return app_domain_get_root_domain_reply;
}

const Reply *Agent::Process(const AppDomainGetCorlibCommand *command)
{
    AppDomainGetCorlibCommand::Reply *get_corlib_reply = command->reply();

    Il2CppDomain *domain = command->domain();
    const Il2CppImage *image = il2cpp_get_corlib();

    get_corlib_reply->assembly(const_cast<Il2CppAssembly*>(il2cpp_image_get_assembly(image)));

    return get_corlib_reply;
}

const Reply *Agent::Process(const AppDomainGetFriendlyNameCommand *command)
{
    AppDomainGetFriendlyNameCommand::Reply *get_friendly_name_reply = command->reply();

    Il2CppDomain *domain = command->domain();

    LOG("warning: `AppDomainGetFriendlyNameCommand` not implemented properly. Returning a stub name for now!");

    get_friendly_name_reply->friendly_name("[ROOT_DOMAIN]");

    return get_friendly_name_reply;
}

const Reply *Agent::Process(const AppDomainCreateStringCommand *command)
{
    AppDomainCreateStringCommand::Reply *create_string_reply = command->reply();

    const Il2CppDomain *domain = command->domain();
    const std::string str = command->string();

    create_string_reply->object((Il2CppObject*)il2cpp_string_new(str.c_str()));

    return create_string_reply;
}

const Reply *Agent::Process(const AppDomainGetEntryAssemblyCommand *command)
{
    LOG("warning: `AppDomainGetEntryAssemblyCommand` not implemented. Returning a `NotImplemented` reply!");

    IL2CPP_ASSERT(0);

    return new InternalErrorNotImplementedReply(command);
}

const Reply *Agent::Process(const AppDomainCreateBoxedValueCommand *command)
{
    LOG("warning: `AppDomainCreateBoxedValueCommand` not implemented. Returning a `NotImplemented` reply!");

    IL2CPP_ASSERT(0);

    return new InternalErrorNotImplementedReply(command);
}

const Reply *Agent::Process(const AppDomainGetAssembliesCommand *command)
{
    AppDomainGetAssembliesCommand::Reply *get_assemblies_reply = command->reply();

    size_t number_of_assemblies;
    const Il2CppAssembly** assemblies = il2cpp_domain_get_assemblies(NULL, &number_of_assemblies);

    get_assemblies_reply->number_of_assemblies(number_of_assemblies);

    for (size_t i = 0; i < number_of_assemblies; ++i)
        get_assemblies_reply->assemblies().push_back(const_cast<Il2CppAssembly*>(assemblies[i]));

    return get_assemblies_reply;
}

#endif
