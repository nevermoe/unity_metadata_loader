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
    class LIBIL2CPP_CODEGEN_API SafeStringMarshal
    {
    public:
        static Il2CppIntPtr StringToUtf8(Il2CppString* str);
        static void GFree(Il2CppIntPtr ptr);
    };
} // namespace Mono
} // namespace mscorlib
} // namespace icalls
} // namespace il2cpp

#endif
