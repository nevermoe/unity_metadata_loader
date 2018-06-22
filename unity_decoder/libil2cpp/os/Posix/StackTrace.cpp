#include "il2cpp-config.h"

#if IL2CPP_TARGET_POSIX && !IL2CPP_TARGET_JAVASCRIPT && !IL2CPP_TARGET_ANDROID && !IL2CPP_TARGET_PS4

#include "os/StackTrace.h"
#include <execinfo.h>
#include <stdio.h>
#include <cxxabi.h>
#include <dlfcn.h>


namespace il2cpp
{
namespace os
{
    const int kMaxStackFrames = 128;

    void StackTrace::WalkStack(WalkStackCallback callback, void* context, WalkOrder walkOrder)
    {
        void* callstack[kMaxStackFrames];
        int frames = backtrace(callstack, kMaxStackFrames);

        if (walkOrder == kFirstCalledToLastCalled)
        {
            for (size_t i = frames; i--;)
            {
                if (!callback(reinterpret_cast<Il2CppMethodPointer>(callstack[i]), context))
                    break;
            }
        }
        else
        {
            for (size_t i = 0; i < frames; i++)
            {
                if (!callback(reinterpret_cast<Il2CppMethodPointer>(callstack[i]), context))
                    break;
            }
        }
    }
}
}

#endif
