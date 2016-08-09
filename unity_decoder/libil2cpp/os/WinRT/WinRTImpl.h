#pragma once

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
	}
}

extern "C"
{

#define CreateEvent CreateEventW
#define FreeEnvironmentStrings FreeEnvironmentStringsW
#define GetComputerName GetComputerNameW
#define GetEnvironmentStrings GetEnvironmentStringsW
#define GetEnvironmentVariable GetEnvironmentVariableW
#define GetUserName GetUserNameW
#define GetVersionEx GetVersionExW
#define SetEnvironmentVariable SetEnvironmentVariableW

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

inline HANDLE WINAPI CreateEventW(LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset, BOOL bInitialState, LPCWSTR lpName)
{
	DWORD flags = 0;
	if (bManualReset)
		flags |= CREATE_EVENT_MANUAL_RESET;
	if (bInitialState)
		flags |= CREATE_EVENT_INITIAL_SET;
	return CreateEventExW(lpEventAttributes, lpName, flags, EVENT_ALL_ACCESS);
}

inline HANDLE WINAPI CreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	const DWORD kFileAttributeMask = 0x0000FFFF;
	const DWORD kFileFlagMask = 0xFFFF0000;

	CREATEFILE2_EXTENDED_PARAMETERS extendedParameters;
	extendedParameters.dwSize = sizeof(CREATEFILE2_EXTENDED_PARAMETERS);
	extendedParameters.dwFileAttributes = dwFlagsAndAttributes & kFileAttributeMask;
	extendedParameters.dwFileFlags = dwFlagsAndAttributes & kFileFlagMask;
	extendedParameters.dwSecurityQosFlags = SECURITY_ANONYMOUS;
	extendedParameters.lpSecurityAttributes = lpSecurityAttributes;
	extendedParameters.hTemplateFile = hTemplateFile;

	return CreateFile2(lpFileName, dwDesiredAccess, dwShareMode, dwCreationDisposition, &extendedParameters);
}

BOOL WINAPI FreeEnvironmentStringsW(LPWCH strings);

inline UINT WINAPI GetACP()
{
	return CP_ACP;
}

BOOL WINAPI GetComputerNameW(LPWSTR lpBuffer, LPDWORD nSize);

LPWCH WINAPI GetEnvironmentStringsW();

DWORD WINAPI GetEnvironmentVariableW(LPCWSTR lpName, LPWSTR lpBuffer, DWORD nSize);

HANDLE WINAPI GetStdHandle(DWORD nStdHandle);

BOOL WINAPI GetUserNameW(LPWSTR lpBuffer, LPDWORD pcbBuffer);

BOOL WINAPI GetVersionExW(LPOSVERSIONINFOW lpVersionInformation);

inline HMODULE WINAPI LoadLibraryW(LPCWSTR lpLibFileName)
{
	return LoadPackagedLibrary(lpLibFileName, 0);
}

BOOL WINAPI SetEnvironmentVariableW(LPCWSTR lpName, LPCWSTR lpValue);

#define CreateFileMappingW(hFile, lpFileMappingAttributes, flProtect, dwMaximumSizeHigh, dwMaximumSizeLow, lpName) \
	CreateFileMappingFromApp(hFile, lpFileMappingAttributes, flProtect, (static_cast<ULONG64>(dwMaximumSizeHigh) << 32) | dwMaximumSizeLow, lpName);

#define MapViewOfFile(hFileMappingObject, dwDesiredAccess, dwFileOffsetHigh, dwFileOffsetLow, dwNumberOfBytesToMap) \
	MapViewOfFileFromApp(hFileMappingObject, dwDesiredAccess, (static_cast<ULONG64>(dwFileOffsetHigh) << 32) | dwFileOffsetLow, dwNumberOfBytesToMap);

#define TlsAlloc() FlsAlloc(NULL)
#define TlsGetValue FlsGetValue
#define TlsSetValue FlsSetValue
#define TlsFree FlsFree

} // extern "C"
