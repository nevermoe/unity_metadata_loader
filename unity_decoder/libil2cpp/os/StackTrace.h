#pragma once

#include <string>
#include <vector>

namespace il2cpp
{
namespace os
{
    typedef bool(*WalkStackCallback)(Il2CppMethodPointer frame, void* context);

    class StackTrace
    {
    public:
        enum WalkOrder
        {
            kFirstCalledToLastCalled,
            kLastCalledToFirstCalled
        };

        // Walks the stack calling callback for each frame in the stack
        // Stops when callback returns false
        static void WalkStack(WalkStackCallback callback, void* context, WalkOrder walkOrder);
    };
}
}
