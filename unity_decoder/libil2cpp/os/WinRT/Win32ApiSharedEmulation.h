#pragma once

#if IL2CPP_TARGET_WINRT || IL2CPP_TARGET_XBOXONE

#include "os/Win32/WindowsHeaders.h"
#include <wrl.h>

#define MAX_COMPUTERNAME_LENGTH 31

namespace il2cpp
{
namespace winrt
{
    inline DWORD WIN32_FROM_HRESULT(HRESULT hr)
    {
        if ((hr & 0xFFFF0000) == MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, 0))
            return HRESULT_CODE(hr);
        if (hr == S_OK)
            return HRESULT_CODE(hr);
        return ERROR_SUCCESS;
    }

    inline static BOOL CopyHStringToBuffer(Microsoft::WRL::Wrappers::HString& source, LPWSTR target, LPDWORD targetSize)
    {
        unsigned int sourceLength;
        auto sourceBuffer = source.GetRawBuffer(&sourceLength);

        if (sourceLength > *targetSize - 1)
        {
            SetLastError(ERROR_BUFFER_OVERFLOW);
            *targetSize = sourceLength + 1;
            return FALSE;
        }

        memcpy(target, sourceBuffer, sourceLength * sizeof(wchar_t));
        target[sourceLength] = L'\0';
        *targetSize = sourceLength;
        return TRUE;
    }
}
}

extern "C"
{
#define GetComputerName GetComputerNameW

inline BOOL WINAPI CopyFileW(LPCWSTR lpExistingFileName, LPCWSTR lpNewFileName, BOOL bFailIfExists)
{
    COPYFILE2_EXTENDED_PARAMETERS params;

    params.dwSize = sizeof(params);
    params.dwCopyFlags = bFailIfExists ? COPY_FILE_FAIL_IF_EXISTS : 0;
    params.pfCancel = FALSE;
    params.pProgressRoutine = nullptr;
    params.pvCallbackContext = nullptr;

    auto hr = CopyFile2(lpExistingFileName, lpNewFileName, &params);
    if (FAILED(hr))
    {
        SetLastError(il2cpp::winrt::WIN32_FROM_HRESULT(hr));
        return FALSE;
    }

    return TRUE;
}

inline UINT WINAPI GetACP()
{
    return CP_ACP;
}

BOOL WINAPI GetComputerNameW(LPWSTR lpBuffer, LPDWORD nSize);
} // extern "C"

#endif
