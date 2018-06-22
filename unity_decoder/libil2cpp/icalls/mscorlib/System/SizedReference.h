#pragma once

#if NET_4_0

#include "object-internals.h"

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{
    class LIBIL2CPP_CODEGEN_API SizedReference
    {
    public:
        static int64_t GetApproximateSizeOfSizedRef(Il2CppIntPtr h);
        static Il2CppIntPtr CreateSizedRef(Il2CppObject* o);
        static Il2CppObject* GetTargetOfSizedRef(Il2CppIntPtr h);
        static void FreeSizedRef(Il2CppIntPtr h);
    };
} // namespace System
} // namespace mscorlib
} // namespace icalls
} // namespace il2cpp

#endif
