#include "il2cpp-config.h"

#if IL2CPP_TARGET_POSIX && !IL2CPP_TARGET_DARWIN

#include <sys/types.h>
#include <unistd.h>

#include "os/Process.h"
#include "vm/Exception.h"

#include "il2cpp-vm-support.h"

struct ProcessHandle
{
    pid_t pid;
};

namespace il2cpp
{
namespace os
{
    int Process::GetCurrentProcessId()
    {
        return getpid();
    }

    ProcessHandle* Process::GetProcess(int processId)
    {
        // If/when we implement the CreateProcess_internal icall we will likely
        // need to so something smarter here to find the process if we did
        // not create it and return a known pseudo-handle. For now this
        // is sufficient though.
        return (ProcessHandle*)(intptr_t)processId;
    }

    void Process::FreeProcess(ProcessHandle* handle)
    {
        // We have nothing to do here.
    }

    std::string Process::GetProcessName(ProcessHandle* handle)
    {
        IL2CPP_VM_NOT_SUPPORTED(Process::GetProcessName, "GetProcessName is not supported for non-Windows/OSX desktop platforms");
        return std::string();
    }
}
}

#endif
