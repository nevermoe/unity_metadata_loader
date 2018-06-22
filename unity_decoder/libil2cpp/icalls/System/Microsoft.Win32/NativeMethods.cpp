#if NET_4_0
#include "il2cpp-config.h"
#include "NativeMethods.h"
#include "os/Win32/WindowsHeaders.h"
#include "os/NativeMethods.h"
#include "os/Process.h"

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
    bool NativeMethods::CloseProcess(Il2CppIntPtr handle)
    {
        return os::NativeMethods::CloseProcess((il2cpp::os::ProcessHandle*)handle.m_value);
    }

    bool NativeMethods::GetExitCodeProcess(Il2CppIntPtr processHandle, int32_t* exitCode)
    {
        return os::NativeMethods::GetExitCodeProcess((il2cpp::os::ProcessHandle*)processHandle.m_value, exitCode);
    }

    bool NativeMethods::GetProcessTimes(Il2CppIntPtr handle, int64_t* creation, int64_t* exit, int64_t* kernel, int64_t* user)
    {
        NOT_IMPLEMENTED_ICALL(NativeMethods::GetProcessTimes);
        IL2CPP_UNREACHABLE;
        return false;
    }

    bool NativeMethods::GetProcessWorkingSetSize(Il2CppIntPtr handle, Il2CppIntPtr* min, Il2CppIntPtr* max)
    {
        NOT_IMPLEMENTED_ICALL(NativeMethods::GetProcessWorkingSetSize);
        IL2CPP_UNREACHABLE;
        return false;
    }

    bool NativeMethods::SetPriorityClass(Il2CppIntPtr handle, int32_t priorityClass)
    {
        NOT_IMPLEMENTED_ICALL(NativeMethods::SetPriorityClass);
        IL2CPP_UNREACHABLE;
        return false;
    }

    bool NativeMethods::SetProcessWorkingSetSize(Il2CppIntPtr handle, Il2CppIntPtr min, Il2CppIntPtr max)
    {
        NOT_IMPLEMENTED_ICALL(NativeMethods::SetProcessWorkingSetSize);
        IL2CPP_UNREACHABLE;
        return false;
    }

    bool NativeMethods::TerminateProcess(Il2CppIntPtr processHandle, int32_t exitCode)
    {
        NOT_IMPLEMENTED_ICALL(NativeMethods::TerminateProcess);
        IL2CPP_UNREACHABLE;
        return false;
    }

    int32_t NativeMethods::GetCurrentProcessId()
    {
        return os::NativeMethods::GetCurrentProcessId();
    }

    int32_t NativeMethods::GetPriorityClass(Il2CppIntPtr handle)
    {
        NOT_IMPLEMENTED_ICALL(NativeMethods::GetPriorityClass);
        IL2CPP_UNREACHABLE;
        return 0;
    }

    int32_t NativeMethods::WaitForInputIdle(Il2CppIntPtr handle, int32_t milliseconds)
    {
        NOT_IMPLEMENTED_ICALL(NativeMethods::WaitForInputIdle);
        IL2CPP_UNREACHABLE;
        return 0;
    }

    Il2CppIntPtr NativeMethods::GetCurrentProcess()
    {
        Il2CppIntPtr handle;
        handle.m_value = os::NativeMethods::GetCurrentProcess();
        return handle;
    }
} // namespace Win32
} // namespace Microsoft
} // namespace System
} // namespace icalls
} // namespace il2cpp
#endif
