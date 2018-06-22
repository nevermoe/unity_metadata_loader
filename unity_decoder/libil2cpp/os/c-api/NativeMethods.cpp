#include "os/NativeMethods.h"
#include "os/c-api/Process-c-api.h"

extern "C"
{
bool UnityPalNativeCloseProcess(UnityPalProcessHandle* handle)
{
    return il2cpp::os::NativeMethods::CloseProcess(handle);
}

bool UnityPalNativeGetExitCodeProcess(UnityPalProcessHandle* handle, int32_t* exitCode)
{
    return il2cpp::os::NativeMethods::GetExitCodeProcess(handle, exitCode);
}

int32_t UnityPalNativeGetCurrentProcessId()
{
    return il2cpp::os::NativeMethods::GetCurrentProcessId();
}

UnityPalProcessHandle* UnityPalNativeGetCurrentProcess()
{
    return il2cpp::os::NativeMethods::GetCurrentProcess();
}
}
