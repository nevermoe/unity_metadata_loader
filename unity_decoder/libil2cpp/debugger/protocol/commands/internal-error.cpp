#if IL2CPP_DEBUGGER_ENABLED

#include "internal-error.h"

using namespace il2cpp::debugger;

InternalErrorNotImplementedCommand::InternalErrorNotImplementedCommand(int32_t id) :
    Command(kCommandSetInternalError, kCommandSetInternalErrorNotImplemented, id)
{
}

InternalErrorNotImplementedReply::InternalErrorNotImplementedReply(const Command *command) :
    Reply(command)
{
    _error_code = kErrorCodeNotImplemented;
}

#endif
