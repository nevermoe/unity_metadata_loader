#pragma once

#if IL2CPP_PLATFORM_WIN32

#include "WindowsHeaders.h"
#include "os/WaitStatus.h"

#if IL2CPP_TARGET_WINRT
#include "os/WinRT/WinRTImpl.h"
#endif
#if IL2CPP_TARGET_XBOXONE
#include "os/XboxOne/Win32ApiEmulator.h"
#endif

namespace il2cpp
{
namespace os
{
namespace win
{

// Wait for a release of the given handle in way that can be interrupted by APCs.
WaitStatus WaitForSingleObjectAndAccountForAPCs (HANDLE handle, uint32_t ms, bool interruptible);

}
}
}

#endif // IL2CPP_TARGET_WINDOWS
