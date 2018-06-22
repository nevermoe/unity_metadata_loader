#pragma once

#if NET_4_0
#include "object-internals.h"

namespace il2cpp
{
namespace icalls
{
namespace System
{
namespace Microsoft
{
namespace Win32
{
    class LIBIL2CPP_CODEGEN_API NativeMethods
    {
    public:
        static bool CloseProcess(Il2CppIntPtr handle);
        static bool GetExitCodeProcess(Il2CppIntPtr processHandle, int32_t* exitCode);
        static bool GetProcessTimes(Il2CppIntPtr handle, int64_t* creation, int64_t* exit, int64_t* kernel, int64_t* user);
        static bool GetProcessWorkingSetSize(Il2CppIntPtr handle, Il2CppIntPtr* min, Il2CppIntPtr* max);
        static bool SetPriorityClass(Il2CppIntPtr handle, int32_t priorityClass);
        static bool SetProcessWorkingSetSize(Il2CppIntPtr handle, Il2CppIntPtr min, Il2CppIntPtr max);
        static bool TerminateProcess(Il2CppIntPtr processHandle, int32_t exitCode);
        static int32_t GetCurrentProcessId();
        static int32_t GetPriorityClass(Il2CppIntPtr handle);
        static int32_t WaitForInputIdle(Il2CppIntPtr handle, int32_t milliseconds);
        static Il2CppIntPtr GetCurrentProcess();
    };
} // namespace Win32
} // namespace Microsoft
} // namespace System
} // namespace icalls
} // namespace il2cpp
#endif
