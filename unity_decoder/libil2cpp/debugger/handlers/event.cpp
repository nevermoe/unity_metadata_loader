#if IL2CPP_DEBUGGER_ENABLED

#pragma once

#include "../../il2cpp-api.h"

#include "../agent.h"
#include "../protocol/commands/event.h"
#include "../protocol/commands/internal-error.h"

namespace il2cpp
{
namespace debugger
{
    const Reply *Agent::Process(const EventCompositeCommand *command)
    {
        LOG("warning: `EventCompositeCommand` not implemented. Returning a `NotImplemented` reply!");

        IL2CPP_ASSERT(0);

        return new InternalErrorNotImplementedReply(command);
    }
}
}

#endif
