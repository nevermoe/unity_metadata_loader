#pragma once

#if defined(__cplusplus)

typedef il2cpp::os::ProcessHandle UnityPalProcessHandle;

#else

typedef struct UnityPalProcessHandle UnityPalProcessHandle;

#endif //__cplusplus

#if defined(__cplusplus)
extern "C"
{
#endif

int UnityPalGetCurrentProcessId();
UnityPalProcessHandle* UnityPalGetProcess(int processId);
void UnityPalFreeProcess(UnityPalProcessHandle* handle);
const char* UnityPalGetProcessName(UnityPalProcessHandle* handle);

#if defined(__cplusplus)
}
#endif
