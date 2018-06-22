#pragma once
#if NET_4_0
#include "object-internals.h"

struct Il2CppInternalThread;

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{
namespace Threading
{
    class LIBIL2CPP_CODEGEN_API InternalThread
    {
    public:
        static void Thread_free_internal(Il2CppInternalThread* _this, Il2CppIntPtr handle);
    };
} // namespace Threading
} // namespace System
} // namespace mscorlib
} // namespace icalls
} // namespace il2cpp
#endif
