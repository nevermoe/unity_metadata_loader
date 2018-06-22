#if IL2CPP_DEBUGGER_ENABLED

#pragma once

#include "../../il2cpp-api.h"

#include "../agent.h"
#include "../protocol/commands/internal-error.h"

namespace il2cpp
{
namespace debugger
{
    const Reply *Agent::Process(const InternalErrorNotImplementedCommand *command)
    {
        return new InternalErrorNotImplementedReply(command);
    }
}
}

#endif
