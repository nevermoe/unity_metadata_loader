#include "il2cpp-config.h"

#if IL2CPP_TARGET_WINDOWS

#include "WindowsHeaders.h"

#include "os/Mutex.h"
#include "os/StackTrace.h"

namespace il2cpp
{
namespace os
{
    void StackTrace::WalkStack(WalkStackCallback callback, void* context, WalkOrder walkOrder)
    {
        const uint32_t kMaxFrames = 128;
        void* stack[kMaxFrames];

        size_t frames = CaptureStackBackTrace(0, kMaxFrames, stack, NULL);

        if (walkOrder == WalkOrder::kFirstCalledToLastCalled)
        {
            for (size_t i = frames; i--;)
            {
                if (!callback(reinterpret_cast<Il2CppMethodPointer>(stack[i]), context))
                    break;
            }
        }
        else
        {
            for (size_t i = 0; i < frames; i++)
            {
                if (!callback(reinterpret_cast<Il2CppMethodPointer>(stack[i]), context))
                    break;
            }
        }
    }
}
}

#endif
