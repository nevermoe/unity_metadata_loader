#if IL2CPP_DEBUGGER_ENABLED

#include "array-ref.h"

using namespace il2cpp::debugger;

void ArrayRefGetValuesCommand::Reply::WriteContentTo(Buffer &out) const
{
    for (std::vector<Variant>::const_iterator it = _values.begin(); it != _values.end(); ++it)
    {
        out.WriteVariant(*it);
    }
}

#endif
