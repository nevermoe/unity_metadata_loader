#pragma once
#if NET_4_0
#include "object-internals.h"

namespace il2cpp
{
namespace icalls
{
namespace System
{
namespace System
{
namespace IO
{
    class LIBIL2CPP_CODEGEN_API KqueueMonitor
    {
    public:
        static int32_t kevent_notimeout(int32_t* kq, Il2CppIntPtr ev, int32_t nchanges, Il2CppIntPtr evtlist, int32_t nevents);
    };
} // namespace IO
} // namespace System
} // namespace System
} // namespace icalls
} // namespace il2cpp
#endif
