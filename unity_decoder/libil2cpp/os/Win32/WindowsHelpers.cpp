#include "il2cpp-config.h"
#include "WindowsHelpers.h"
#include "os/Time.h"

#if IL2CPP_TARGET_WINDOWS

namespace il2cpp
{
namespace os
{
namespace win
{
    WaitStatus WaitForSingleObjectAndAccountForAPCs(HANDLE handle, uint32_t ms, bool interruptible)
    {
        uint32_t remainingWaitTimeMS = ms;
        while (true)
        {
            uint32_t waitStartTime = os::Time::GetTicksMillisecondsMonotonic();
            DWORD result = ::WaitForSingleObjectEx(handle, remainingWaitTimeMS, interruptible);

            if (result == WAIT_OBJECT_0)
                return kWaitStatusSuccess;

            if (result == WAIT_TIMEOUT)
                return kWaitStatusTimeout;

            if (result == WAIT_IO_COMPLETION)
            {
                if (ms != INFINITE)
                {
                    uint32_t haveWaitedTimeMS = os::Time::GetTicksMillisecondsMonotonic() - waitStartTime;
                    if (haveWaitedTimeMS >= remainingWaitTimeMS)
                        return kWaitStatusTimeout;
                    remainingWaitTimeMS -= haveWaitedTimeMS;
                }
                continue;
            }

            break;
        }

        return kWaitStatusFailure;
    }
}
}
}

#endif // IL2CPP_TARGET_WINDOWS
