#if IL2CPP_DEBUGGER_ENABLED

#pragma once

#include "../../il2cpp-api.h"

#include "../agent.h"
#include "../protocol/commands/module.h"

namespace il2cpp
{
namespace debugger
{
    const Reply *Agent::Process(const ModuleGetInfoCommand *command)
    {
        ModuleGetInfoCommand::Reply *get_info_reply = command->reply();

        Il2CppImage *image = command->module();

        LOG("warning: `ModuleGetInfoCommand` is not implemented properly.");

        get_info_reply->basename("FAKE BASENAME");
        get_info_reply->scope_name("FAKE SCOPE NAME");
        get_info_reply->fullname(il2cpp_image_get_name(image));
        get_info_reply->guid("FAKE GUID");
        get_info_reply->assembly(const_cast<Il2CppAssembly*>(il2cpp_image_get_assembly(image)));

        return get_info_reply;
    }
}
}

#endif
