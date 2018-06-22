#pragma once

#include "../common.h"

#include "globals.h"

namespace il2cpp
{
namespace debugger
{

const char* event_to_string(EventKind evt);
const char* command_set_to_string(CommandSet command_set);
const char* command_to_string(CommandSet command_set, int command);

} /* namespace debugger */
} /* namespace il2cpp */
