#include "il2cpp-config.h"

// Generic implementation for platforms that don't have this implemented
#if !IL2CPP_ISDEBUGGERPRESENT_IMPLEMENTED

#include "os/Debug.h"

namespace il2cpp
{
namespace os
{
    bool Debug::IsDebuggerPresent()
    {
        return false;
    }
}
}

#endif
