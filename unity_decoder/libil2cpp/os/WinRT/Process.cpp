#include "il2cpp-config.h"

#if IL2CPP_TARGET_WINRT || IL2CPP_TARGET_XBOXONE
#include "os/Win32/WindowsHeaders.h"

#include "os/Process.h"
#include "utils/StringUtils.h"
#include "vm/Exception.h"

struct ProcessHandle
{
    HANDLE handle;
};


namespace il2cpp
{
namespace os
{
    int Process::GetCurrentProcessId()
    {
        return ::GetCurrentProcessId();
    }

    ProcessHandle* Process::GetProcess(int processId)
    {
        if (processId == GetCurrentProcessId())
            return (ProcessHandle*)::GetCurrentProcess();

        vm::Exception::Raise(vm::Exception::GetPlatformNotSupportedException(L"It is not possible to interact with other system processes on current platform."));
    }

    void Process::FreeProcess(ProcessHandle* handle)
    {
        // We have nothing to do here.
    }

    std::string Process::GetProcessName(ProcessHandle* handle)
    {
        if (handle == ::GetCurrentProcess())
        {
            wchar_t path[MAX_PATH + 1];
            SetLastError(ERROR_SUCCESS);

            DWORD pathLength = GetModuleFileNameW(NULL, path, MAX_PATH + 1);
            return utils::StringUtils::Utf16ToUtf8(path, static_cast<int>(pathLength));
        }

        vm::Exception::Raise(vm::Exception::GetPlatformNotSupportedException(L"It is not possible to interact with other system processes on current platform."));
    }
}
}
#endif
