#include "il2cpp-config.h"

#if IL2CPP_TARGET_WINDOWS

#include "WindowsHeaders.h"

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

namespace il2cpp
{
namespace os
{
namespace Image
{
    void* GetImageBase()
    {
        return &__ImageBase;
    }
}
}
}

#endif
