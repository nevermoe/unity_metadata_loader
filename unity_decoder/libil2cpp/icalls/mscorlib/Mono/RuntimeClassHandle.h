#pragma once

#if NET_4_0

#include "object-internals.h"

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace Mono
{
    class LIBIL2CPP_CODEGEN_API RuntimeClassHandle
    {
    public:
        static Il2CppIntPtr GetTypeFromClass(Il2CppClass* klass);
    };
} // namespace Mono
} // namespace mscorlib
} // namespace icalls
} // namespace il2cpp

#endif
