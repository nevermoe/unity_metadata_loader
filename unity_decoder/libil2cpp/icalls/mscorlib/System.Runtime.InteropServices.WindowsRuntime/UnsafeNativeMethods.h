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
namespace Runtime
{
namespace InteropServices
{
namespace WindowsRuntime
{
    class LIBIL2CPP_CODEGEN_API UnsafeNativeMethods
    {
    public:
        static bool RoOriginateLanguageException(int32_t error, Il2CppString* message, Il2CppIntPtr languageException);
        static Il2CppChar* WindowsGetStringRawBuffer(Il2CppIntPtr hstring, uint32_t* length);
        static int32_t WindowsCreateString(Il2CppString* sourceString, int32_t length, Il2CppIntPtr* hstring);
        static int32_t WindowsDeleteString(Il2CppIntPtr hstring);
        static Il2CppObject* GetRestrictedErrorInfo();
        static void RoReportUnhandledError(Il2CppObject* error);
    };
} // namespace WindowsRuntime
} // namespace InteropServices
} // namespace Runtime
} // namespace System
} // namespace mscorlib
} // namespace icalls
} // namespace il2cpp

#endif
