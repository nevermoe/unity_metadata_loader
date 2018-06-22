#include "il2cpp-config.h"

#include "icalls/System/System.Diagnostics/Process.h"
#include "os/Process.h"
#include "vm/Exception.h"
#include "vm/String.h"

namespace il2cpp
{
namespace icalls
{
namespace System
{
namespace System
{
namespace Diagnostics
{
    int32_t Process::ExitCode_internal(Il2CppIntPtr handle)
    {
        NOT_IMPLEMENTED_ICALL(Process::ExitCode_internal);

        return 0;
    }

    int64_t Process::ExitTime_internal(Il2CppIntPtr handle)
    {
        NOT_IMPLEMENTED_ICALL(Process::ExitTime_internal);

        return 0;
    }

    bool Process::GetWorkingSet_internal(Il2CppIntPtr handle, int32_t* min, int32_t* max)
    {
        NOT_IMPLEMENTED_ICALL(Process::GetWorkingSet_internal);

        return false;
    }

    bool Process::SetWorkingSet_internal(Il2CppIntPtr handle, int32_t min, int32_t max, bool use_min)
    {
        NOT_IMPLEMENTED_ICALL(Process::SetWorkingSet_internal);

        return false;
    }

    Il2CppArray* Process::GetModules_internal(void* /* System.Diagnostics.Process */ self, Il2CppIntPtr handle)
    {
        NOT_IMPLEMENTED_ICALL(Process::GetModules_internal);

        return 0;
    }

    int64_t Process::GetProcessData(int32_t pid, int32_t data_type, int32_t* error)
    {
        NOT_IMPLEMENTED_ICALL(Process::GetProcessData);

        return 0;
    }

    int32_t Process::GetPriorityClass(Il2CppIntPtr handle, int32_t* error)
    {
        NOT_IMPLEMENTED_ICALL(Process::GetPriorityClass);

        return 0;
    }

    bool Process::SetPriorityClass(Il2CppIntPtr handle, int32_t priority, int32_t* error)
    {
        NOT_IMPLEMENTED_ICALL(Process::SetPriorityClass);

        return false;
    }

    int64_t Process::Times(Il2CppIntPtr handle, int32_t type)
    {
        NOT_IMPLEMENTED_ICALL(Process::Times);

        return 0;
    }

    Il2CppString* Process::ProcessName_internal(Il2CppIntPtr handle)
    {
        os::ProcessHandle *pHandle = (os::ProcessHandle*)handle.m_value;
        std::string name = os::Process::GetProcessName(pHandle);
        return il2cpp::vm::String::New(name.c_str());
    }

    int64_t Process::StartTime_internal(Il2CppIntPtr handle)
    {
        NOT_IMPLEMENTED_ICALL(Process::StartTime_internal);

        return 0;
    }

    bool Process::Kill_internal(Il2CppIntPtr handle, int32_t signo)
    {
        NOT_IMPLEMENTED_ICALL(Process::Kill_internal);

        return false;
    }

    Il2CppIntPtr Process::GetProcess_internal(int32_t pid)
    {
        Il2CppIntPtr process;
        process.m_value = os::Process::GetProcess(pid);

        return process;
    }

    int32_t Process::GetPid_internal()
    {
        return os::Process::GetCurrentProcessId();
    }

    Il2CppArray* Process::GetProcesses_internal()
    {
        NOT_IMPLEMENTED_ICALL(Process::GetProcesses_internal);

        return 0;
    }

    bool Process::ShellExecuteEx_internal(void* /* System.Diagnostics.ProcessStartInfo */ startInfo, ProcInfo* proc_info)
    {
        NOT_IMPLEMENTED_ICALL(Process::ShellExecuteEx_internal);

        return false;
    }

    bool Process::CreateProcess_internal(void* /* System.Diagnostics.ProcessStartInfo */ startInfo, Il2CppIntPtr _stdin, Il2CppIntPtr _stdout, Il2CppIntPtr _stderr, ProcInfo* proc_info)
    {
        NOT_IMPLEMENTED_ICALL(Process::CreateProcess_internal);

        return false;
    }

    bool Process::WaitForExit_internal(void* /* System.Diagnostics.Process */ self, Il2CppIntPtr handle, int32_t ms)
    {
        NOT_IMPLEMENTED_ICALL(Process::WaitForExit_internal);

        return false;
    }

    bool Process::WaitForInputIdle_internal(void* /* System.Diagnostics.Process */ self, Il2CppIntPtr handle, int32_t ms)
    {
        NOT_IMPLEMENTED_ICALL(Process::WaitForInputIdle_internal);

        return false;
    }

    void Process::Process_free_internal(void* /* System.Diagnostics.Process */ self, Il2CppIntPtr handle)
    {
        os::Process::FreeProcess((os::ProcessHandle*)handle.m_value);
    }
} /* namespace Diagnostics */
} /* namespace System */
} /* namespace System */
} /* namespace icalls */
} /* namespace il2cpp */
