#pragma once

#include "../../common.h"
#include "../../meta.h"
#include "../protocol.h"

namespace il2cpp
{
namespace debugger
{

struct InternalErrorNotImplementedCommand : public Command
{

public:

	explicit InternalErrorNotImplementedCommand(int32_t id);

	PROTOCOL_COMMAND(InternalError, NotImplemented);

	DISALLOW_COPY(InternalErrorNotImplementedCommand);
};

struct InternalErrorNotImplementedReply : public Reply
{

public:

	explicit InternalErrorNotImplementedReply(const Command *command);

	DISALLOW_COPY(InternalErrorNotImplementedReply);
};

}
}
