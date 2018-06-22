#if IL2CPP_DEBUGGER_ENABLED

#pragma once

#include "../../il2cpp-api.h"

#include "../agent.h"
#include "../protocol/commands/method.h"
#include "../protocol/commands/internal-error.h"

namespace il2cpp
{
namespace debugger
{
    const Reply *Agent::Process(const MethodGetParamInfoCommand *command)
    {
        MethodGetParamInfoCommand::Reply *get_param_info_reply = command->reply();

        LOG("warning: `MethodGetParamInfoCommand` not implemented properly yet.");

        const MethodInfo *method = command->method();

        get_param_info_reply->call_convention(0 /* IL2CPP_CALL_DEFAULT */);
        get_param_info_reply->generic_param_count(0);
        get_param_info_reply->return_type(
            il2cpp_class_from_type(
                (Il2CppType*)il2cpp_method_get_return_type(method)));

        const size_t count = il2cpp_method_get_param_count(method);
        for (uint32_t i = 0; i < count; ++i)
        {
            MethodGetParamInfoCommand::Reply::ParamInfo param;

            param.type = il2cpp_class_from_type(
                    (Il2CppType*)il2cpp_method_get_param(method, i));
            param.name = il2cpp_method_get_param_name(method, i);

            get_param_info_reply->params().push_back(param);
        }

        return get_param_info_reply;
    }

    const Reply *Agent::Process(const MethodGetNameCommand *command)
    {
        MethodGetNameCommand::Reply *get_name_reply = command->reply();

        get_name_reply->name(il2cpp_method_get_name(command->method()));

        return get_name_reply;
    }

    const Reply *Agent::Process(const MethodGetDeclaringTypeCommand *command)
    {
        MethodGetDeclaringTypeCommand::Reply *get_declaring_type_reply = command->reply();

        get_declaring_type_reply->type(il2cpp_method_get_declaring_type(command->method()));

        return get_declaring_type_reply;
    }

    const Reply *Agent::Process(const MethodGetDebugInfoCommand *command)
    {
        MethodGetDebugInfoCommand::Reply *get_debug_info_reply = command->reply();

        const Il2CppDebugMethodInfo *info = il2cpp_debug_get_method_info(command->method());
        const Il2CppDebugDocument *document = il2cpp_debug_method_get_document(info);

        if (info == 0 || document == 0)
            return get_debug_info_reply;

        std::string filename;

        filename += il2cpp_debug_document_get_directory(document);
        filename += il2cpp_debug_document_get_filename(document);

        const int32_t *offset_table = il2cpp_debug_method_get_offset_table(info);

        get_debug_info_reply->code_size(il2cpp_debug_method_get_code_size(info));
        get_debug_info_reply->file_name(filename);

        for (int32_t i = 0; offset_table[i] >= 0; i += 2)
        {
            get_debug_info_reply->offsets().push_back(std::pair<int32_t, int32_t>(offset_table[i], offset_table[i + 1]));
        }

        return get_debug_info_reply;
    }

    const Reply *Agent::Process(const MethodGetBodyCommand *command)
    {
        MethodGetBodyCommand::Reply *get_body_reply = command->reply();

        LOG("warning: `MethodGetBodyCommand` always return an empty byte array for now");

        return get_body_reply;
    }

    const Reply *Agent::Process(const MethodResolveTokenCommand *command)
    {
        LOG("warning: `MethodResolveTokenCommand` not implemented. Returning a `NotImplemented` reply!");

        IL2CPP_ASSERT(0);

        return new InternalErrorNotImplementedReply(command);
    }

    const Reply *Agent::Process(const MethodGetLocalsInfoCommand *command)
    {
        MethodGetLocalsInfoCommand::Reply *get_locals_info_reply = command->reply();

        const Il2CppDebugMethodInfo *info = il2cpp_debug_get_method_info(command->method());
        const Il2CppDebugLocalsInfo **locals = il2cpp_debug_method_get_locals_info(info);

        while (*locals != NULL)
        {
            MethodGetLocalsInfoCommand::Reply::LocalInfo local;

            local.type = il2cpp_debug_local_get_type(*locals);
            local.name = il2cpp_debug_local_get_name(*locals);
            local.begin_scope = il2cpp_debug_local_get_start_offset(*locals);
            local.end_scope = il2cpp_debug_local_get_end_offset(*locals);

            if (local.end_scope == 0)
            {
                local.begin_scope = 0;
                local.end_scope = il2cpp_debug_method_get_code_size(info);
            }

            get_locals_info_reply->locals().push_back(local);

            ++locals;
        }

        return get_locals_info_reply;
    }

    const Reply *Agent::Process(const MethodGetInfoCommand *command)
    {
        MethodGetInfoCommand::Reply *reply = command->reply();

        uint32_t iflags = 0;
        reply->flags(
            il2cpp_method_get_flags(command->method(), &iflags));
        reply->token(
            il2cpp_method_get_token(command->method()));
        reply->implementation_flags(iflags);

        return reply;
    }
}
}

#endif
