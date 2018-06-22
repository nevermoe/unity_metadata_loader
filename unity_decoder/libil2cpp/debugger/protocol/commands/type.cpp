#if IL2CPP_DEBUGGER_ENABLED

#include "type.h"

using namespace il2cpp::debugger;

void TypeGetValuesCommand::Reply::WriteContentTo(Buffer &out) const
{
    for (std::vector<Variant>::const_iterator it = _values.begin(); it != _values.end(); ++it)
    {
        out.WriteVariant(*it);
    }
}

#endif
