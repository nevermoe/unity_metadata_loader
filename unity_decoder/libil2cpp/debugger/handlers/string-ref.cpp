#if IL2CPP_DEBUGGER_ENABLED

#pragma once

#include "../../il2cpp-api.h"
#include "../../utils/StringUtils.h"

#include "../agent.h"
#include "../protocol/commands/string-ref.h"
#include "../protocol/commands/internal-error.h"

namespace il2cpp
{
namespace debugger
{
    const Reply *Agent::Process(const StringRefGetValueCommand *command)
    {
        StringRefGetValueCommand::Reply *get_value_reply = command->reply();

        Il2CppString *str = (Il2CppString*)command->object();
        if (str == 0)
            return get_value_reply;

        get_value_reply->value(
            utils::StringUtils::Utf16ToUtf8(
                il2cpp_string_chars(str)));

        return get_value_reply;
    }
}
}

#endif
