#if IL2CPP_DEBUGGER_ENABLED

#include "thread.h"

using namespace il2cpp::debugger;

void ThreadGetFrameInfoCommand::Reply::WriteContentTo(Buffer &out) const
{
    std::vector<FrameInfo>::const_iterator it = _infos.begin();

    out.WriteInt(_infos.size());

    while (it != _infos.end())
    {
        out.WriteInt((*it).frame_id);
        out.WriteMethodId((*it).method);
        out.WriteInt((*it).il_offset);
        out.WriteByte((*it).flags);
        ++it;
    }
}

#endif
