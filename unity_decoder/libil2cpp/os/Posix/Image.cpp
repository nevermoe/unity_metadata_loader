#include "il2cpp-config.h"

#if IL2CPP_TARGET_JAVASCRIPT || IL2CPP_TARGET_LINUX || IL2CPP_TARGET_ANDROID || IL2CPP_TARGET_TIZEN

#if !IL2CPP_TARGET_JAVASCRIPT
#include <dlfcn.h>
#endif

namespace il2cpp
{
namespace os
{
namespace Image
{
    void* GetImageBase()
    {
#if IL2CPP_TARGET_JAVASCRIPT
        return NULL;
#else
        Dl_info info;
        void* const anySymbol = reinterpret_cast<void*>(&GetImageBase);
        if (dladdr(anySymbol, &info))
            return info.dli_fbase;
        else
            return NULL;
#endif
    }
}
}
}

#endif
