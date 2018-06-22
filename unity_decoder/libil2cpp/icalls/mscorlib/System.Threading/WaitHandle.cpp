#include "il2cpp-config.h"
#include <vector>
#include <algorithm>
#include "icalls/mscorlib/System.Threading/WaitHandle.h"
#include "os/Mutex.h"
#include "os/Thread.h"
#include "vm/Thread.h"
#include "vm/Exception.h"
#include "vm/Array.h"
#include "vm/WaitHandle.h"

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{
namespace Threading
{
    std::vector<os::Handle*> ExtractOsWaitHandles(Il2CppArray* handles)
    {
        uint32_t numberOfHandles = vm::Array::GetLength(handles);

        std::vector<os::Handle*> osWaitHandles(numberOfHandles);

        for (uint32_t i = 0; i < numberOfHandles; ++i)
        {
            Il2CppObject* handle = il2cpp_array_get(handles, Il2CppObject*, i);
            Il2CppWaitHandle* waitHandle = (Il2CppWaitHandle*)handle;
            osWaitHandles[i] = vm::WaitHandle::GetPlatformHandle(waitHandle);
        }

        return osWaitHandles;
    }

    int32_t WaitHandle::WaitAny_internal(Il2CppArray* handles, int32_t ms, bool exitContext)
    {
        std::vector<os::Handle*> osWaitHandles = ExtractOsWaitHandles(handles);

        vm::ThreadStateSetter state(vm::kThreadStateWaitSleepJoin);

        int timeWaitedMs = 0;
        while (ms == -1 || timeWaitedMs <= ms)
        {
            int32_t numberOfOsHandles = (int32_t)osWaitHandles.size();
            for (int32_t i = 0; i < numberOfOsHandles; ++i)
            {
                if (osWaitHandles[i]->Wait(0))
                    return i;
            }

            os::Thread::Sleep(m_waitIntervalMs, true);
            timeWaitedMs += m_waitIntervalMs;
        }

        return 258; // WAIT_TIMEOUT value
    }

    bool WaitHandle::WaitOne_internal(Il2CppObject* unused, Il2CppIntPtr handlePtr, int32_t ms, bool exitContext)
    {
        il2cpp::os::Handle* handle = (il2cpp::os::Handle*)handlePtr.m_value;

        vm::ThreadStateSetter state(vm::kThreadStateWaitSleepJoin);

        // Check for pending interrupt before we go waiting.
        il2cpp::vm::Thread::CheckCurrentThreadForInterruptAndThrowIfNecessary();

        if (ms == -1)
            return handle->Wait();

        return handle->Wait(ms);
    }

    bool WaitHandle::WaitAll_internal(Il2CppArray* handles, int32_t ms, bool exitContext)
    {
        std::vector<os::Handle*> osWaitHandles = ExtractOsWaitHandles(handles);

        vm::ThreadStateSetter state(vm::kThreadStateWaitSleepJoin);

        int timeWaitedMs = 0;
        while (ms == -1 || timeWaitedMs <= ms)
        {
            size_t numberOfOsHandles = osWaitHandles.size();
            std::vector<os::Handle*> signaledHandles;
            for (size_t i = 0; i < numberOfOsHandles; ++i)
            {
                if (osWaitHandles[i]->Wait(0))
                    signaledHandles.push_back(osWaitHandles[i]);
            }

            if (signaledHandles.size() == numberOfOsHandles)
                return true; // All handles have been signaled

            for (size_t i = 0; i < signaledHandles.size(); ++i)
                osWaitHandles.erase(std::remove(osWaitHandles.begin(), osWaitHandles.end(), signaledHandles[i]), osWaitHandles.end());

            os::Thread::Sleep(m_waitIntervalMs, true);
            timeWaitedMs += m_waitIntervalMs;
        }

        return false; // Timed out waiting for all handles to be signaled
    }

    bool WaitHandle::SignalAndWait_Internal(Il2CppIntPtr toSignal, Il2CppIntPtr toWaitOn, int32_t ms, bool exitContext)
    {
        os::Handle* toSignalOsHandle = (os::Handle*)toSignal.m_value;
        toSignalOsHandle->Signal();

        return WaitOne_internal(NULL, toWaitOn, ms, exitContext);
    }

#if NET_4_0
    int32_t WaitHandle::SignalAndWait_Internal40(Il2CppIntPtr toSignal, Il2CppIntPtr toWaitOn, int32_t ms)
    {
        return SignalAndWait_Internal(toSignal, toWaitOn, ms, false) ? 0 : 1;
    }

    int32_t WaitHandle::WaitAll_internal40(Il2CppArray* handles, int32_t ms)
    {
        return WaitAll_internal(handles, ms, false) ? 0 : 258;
    }

    int32_t WaitHandle::WaitAny_internal40(Il2CppArray* handles, int32_t ms)
    {
        return WaitAny_internal(handles, ms, false);
    }

    int32_t WaitHandle::WaitOne_internal40(Il2CppIntPtr handle, int32_t ms)
    {
        return WaitOne_internal(NULL, handle, ms, false) ? 0 : 258;
    }

#endif
} /* namespace Threading */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
