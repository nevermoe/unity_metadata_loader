#if IL2CPP_DEBUGGER_ENABLED

#pragma once

#include <sstream>

#include "../../il2cpp-api.h"
#include "../../tabledefs.h"
#include "../../class-internals.h"

#include "../agent.h"
#include "../protocol/commands/assembly.h"
#include "../protocol/commands/internal-error.h"

#include "../../vm/Assembly.h"
#include "../../vm/AssemblyName.h"

namespace il2cpp
{
namespace debugger
{
    const Reply *Agent::Process(const AssemblyGetTypeCommand *command)
    {
        AssemblyGetTypeCommand::Reply *get_type_reply = command->reply();

        /*char *s = decode_string (p, &p, end);
        gboolean ignorecase = decode_byte (p, &p, end);
        MonoTypeNameParse info;
        MonoType *t;
        gboolean type_resolve;

        if (!mono_reflection_parse_type (s, &info)) {
            t = NULL;
        } else {
            if (info.assembly.name)
                NOT_IMPLEMENTED;
            t = mono_reflection_get_type (ass->image, &info, ignorecase, &type_resolve);
        }
        buffer_add_typeid (buf, domain, t ? mono_class_from_mono_type (t) : NULL);
        mono_reflection_free_type_info (&info);
        g_free (s);*/

        LOG("warning: Implement proper name parsing for `AssemblyGetType` command");

        if (command->ignore_case())
            LOG("warning: AssemblyGetType not implemented for ignore_case = true");

        std::string ns;
        std::string name;

        const size_t last_dot_index = command->name().find_last_of(".");

        if (last_dot_index != std::string::npos)
        {
            ns.assign(command->name().begin(), command->name().begin() + last_dot_index);
            name.assign(command->name().begin() + last_dot_index + 1, command->name().end());
        }
        else
        {
            name = command->name();
        }

        Il2CppClass *type = il2cpp_class_from_name(
                il2cpp_assembly_get_image(command->assembly()),
                ns.c_str(), name.c_str());

        if (type == 0)
            LOG("warning: AssemblyGetType command not mocked up for type " << command->name());

        get_type_reply->type(type);

        return get_type_reply;
    }

    const Reply *Agent::Process(const AssemblyGetObjectCommand *command)
    {
        LOG("warning: `AssemblyGetObjectCommand` not implemented. Returning a `NotImplemented` reply!");

        IL2CPP_ASSERT(0);

        return new InternalErrorNotImplementedReply(command);
    }

    const Reply *Agent::Process(const AssemblyGetEntryPointCommand *command)
    {
        LOG("warning: `AssemblyGetEntryPointCommand` not implemented. Returning a `NotImplemented` reply!");

        IL2CPP_ASSERT(0);

        return new InternalErrorNotImplementedReply(command);
    }

    const Reply *Agent::Process(const AssemblyGetManifestModuleCommand *command)
    {
        AssemblyGetManifestModuleCommand::Reply *reply = command->reply();

        reply->module(il2cpp_assembly_get_image(command->assembly()));

        return reply;
    }

    const Reply *Agent::Process(const AssemblyGetNameCommand *command)
    {
        AssemblyGetNameCommand::Reply *reply = command->reply();

        const Il2CppAssembly *assembly = command->assembly();

        reply->name(il2cpp::vm::AssemblyName::AssemblyNameToString(assembly->aname));

        return reply;
    }

    const Reply *Agent::Process(const AssemblyGetLocationCommand *command)
    {
        AssemblyGetLocationCommand::Reply *reply = command->reply();

        reply->location(
            il2cpp_image_get_filename(
                il2cpp_assembly_get_image(command->assembly())));

        return reply;
    }
}
}

#endif
