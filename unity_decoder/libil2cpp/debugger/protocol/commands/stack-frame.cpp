#if IL2CPP_DEBUGGER_ENABLED

#include "stack-frame.h"

using namespace il2cpp::debugger;

void StackFrameGetValuesCommand::Reply::WriteContentTo(Buffer &out) const
{
    for (std::vector<Variant>::const_iterator it = _values.begin(); it != _values.end(); ++it)
    {
        out.WriteVariant(*it);
    }
}

void StackFrameSetValuesCommand::ReadProperties(Buffer &in)
{
    thread.ReadFrom(in);
    frame_id.ReadFrom(in);
    values_count.ReadFrom(in);

    // Note: we need to copy the data, as long as the incoming
    // buffer survives only for the duration of the command parsing.
    // We might want to change this in the future
    _command_buffer = new Buffer(in.length());
    _command_buffer->WriteData(in.raw(), in.length());
    _command_buffer->Seek(in.position());
}

#endif
