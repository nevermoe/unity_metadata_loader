#include "il2cpp-config.h"

#if IL2CPP_TARGET_WINRT

#include <io.h>
#include "os/Win32/WindowsHeaders.h"
#include "os/Mutex.h"
#include "utils/Memory.h"
#include "vm/Exception.h"
#include "vm/Image.h"
#include "WinRTImpl.h"
#include <unordered_map>
#include <windows.networking.connectivity.h>
#include <windows.system.profile.h>
#include <windows.system.userprofile.h>
#include <windows.storage.h>
#include <wrl.h>

#include "SynchronousOperation.h"

using namespace il2cpp::winrt;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;
using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::Foundation::Collections;
using namespace ABI::Windows::Networking;
using namespace ABI::Windows::Networking::Connectivity;
using namespace ABI::Windows::System::Profile;
using namespace ABI::Windows::System::UserProfile;
using namespace ABI::Windows::Storage;
using namespace ABI::Windows::UI::Popups;

typedef std::unordered_map<std::wstring, std::wstring> EnvironmentVariableMap;
static EnvironmentVariableMap s_EnvironmentVariables;
static il2cpp::os::FastMutex s_EnvironmentVariablesMutex;

inline static BOOL CopyHStringToBuffer(HString& source, LPWSTR target, LPDWORD targetSize)
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

extern "C"
{

BOOL WINAPI FreeEnvironmentStringsW(LPWCH strings)
{
	IL2CPP_FREE(strings);
	return TRUE;
}

BOOL WINAPI GetComputerNameW(LPWSTR lpBuffer, LPDWORD nSize)
{
#define ERROR_CHECK(hr) do { if (FAILED(hr)) { SetLastError(WIN32_FROM_HRESULT(hr)); return FALSE; } } while (false)

	ComPtr<INetworkInformationStatics> info;
	auto hr = RoGetActivationFactory(HStringReference(RuntimeClass_Windows_Networking_Connectivity_NetworkInformation).Get(), __uuidof(info), &info);
	ERROR_CHECK(hr);

	ComPtr<IVectorView<HostName*>> names;
	hr = info->GetHostNames(&names);
	ERROR_CHECK(hr);

	unsigned int size;
	hr = names->get_Size(&size);
	if (FAILED(hr) || !size)
	{
		SetLastError(WIN32_FROM_HRESULT(hr));
		return FALSE;
	}

	ComPtr<IHostName> name;
	hr = names->GetAt(0, &name);
	ERROR_CHECK(hr);

	HString displayName;
	hr = name->get_DisplayName(displayName.GetAddressOf());
	ERROR_CHECK(hr);

#undef ERROR_CHECK

	return CopyHStringToBuffer(displayName, lpBuffer, nSize);
}

LPWCH WINAPI GetEnvironmentStringsW()
{
	il2cpp::os::FastAutoLock lock(&s_EnvironmentVariablesMutex);

	// Two iterations
	// 1) Figure out length
	// 2) Make result string
	size_t length = 0;

	for (EnvironmentVariableMap::const_iterator it = s_EnvironmentVariables.cbegin(); it != s_EnvironmentVariables.cend(); it++)
	{
		// Key + value + '=' + '\0'
		length += it->first.length() + it->second.length() + 2;
	}

	// Terminating '\0'
	length++;

	LPWCH result = static_cast<LPWCH>(IL2CPP_MALLOC(length * sizeof(WCHAR)));
	size_t index = 0;

	for (EnvironmentVariableMap::const_iterator it = s_EnvironmentVariables.cbegin(); it != s_EnvironmentVariables.cend(); it++)
	{
		const size_t keyLength = it->first.length();
		const size_t valueLength = it->second.length();

		memcpy(result + index, it->first.c_str(), keyLength * sizeof(WCHAR));
		index += keyLength;

		result[index++] = L'=';

		memcpy(result + index, it->second.c_str(), valueLength * sizeof(WCHAR));
		index += valueLength;

		result[index++] = '\0';
	}

	result[index++] = '\0';
	assert(index == length);
	return result;
}

DWORD WINAPI GetEnvironmentVariableW(LPCWSTR lpName, LPWSTR lpBuffer, DWORD nSize)
{
	il2cpp::os::FastAutoLock lock(&s_EnvironmentVariablesMutex);

	std::wstring key(lpName);
	auto it = s_EnvironmentVariables.find(key);

	if (it == s_EnvironmentVariables.end())
		return 0;

	DWORD sizeNeeded = static_cast<DWORD>(it->second.length());

	if (nSize < sizeNeeded)
	{
		SetLastError(ERROR_BUFFER_OVERFLOW);
	}
	else
	{
		memcpy(lpBuffer, it->second.data(), (sizeNeeded + 1) * sizeof(wchar_t));
	}
	
	return sizeNeeded;
}

HANDLE WINAPI GetStdHandle(DWORD nStdHandle)
{
	FILE* stdFile;
	const wchar_t* fileName;

	switch (nStdHandle)
	{
	case STD_INPUT_HANDLE:
		stdFile = stdin;
		fileName = L"\\stdin.txt";
		break;

	case STD_OUTPUT_HANDLE:
		stdFile = stdout;
		fileName = L"\\stdout.txt";
		break;

	case STD_ERROR_HANDLE:
		stdFile = stderr;
		fileName = L"\\stderr.txt";
		break;

	default:
		return INVALID_HANDLE_VALUE;
	}

	HANDLE stdHandle = reinterpret_cast<HANDLE>(_get_osfhandle(_fileno(stdFile)));
	
	if (stdHandle != INVALID_HANDLE_VALUE)
		return stdHandle;

	ComPtr<IApplicationDataStatics> appDataStatics;
	ComPtr<IApplicationData> appData;
	ComPtr<IStorageFolder> tempFolder;
	ComPtr<IStorageItem> tempFolderItem;
	HString tempPath;
	HString stdFilePath;

	#define ERROR_CHECK(hr) do { if (FAILED(hr)) return INVALID_HANDLE_VALUE; } while (false)

	auto hr = RoGetActivationFactory(HStringReference(RuntimeClass_Windows_Storage_ApplicationData).Get(), __uuidof(IApplicationDataStatics), &appDataStatics);
	ERROR_CHECK(hr);

	hr = appDataStatics->get_Current(&appData);
	ERROR_CHECK(hr);

	hr = appData->get_TemporaryFolder(&tempFolder);
	ERROR_CHECK(hr);

	hr = tempFolder.As(&tempFolderItem);
	ERROR_CHECK(hr);

	hr = tempFolderItem->get_Path(tempPath.GetAddressOf());
	ERROR_CHECK(hr);

	hr = WindowsConcatString(tempPath.Get(), HStringReference(fileName).Get(), stdFilePath.GetAddressOf());
	ERROR_CHECK(hr);

	#undef ERROR_CHECK

	unsigned int dummy;
	auto redirectedFile = _wfreopen(stdFilePath.GetRawBuffer(&dummy), L"w+", stdFile);
	return reinterpret_cast<HANDLE>(_get_osfhandle(_fileno(redirectedFile)));
}

BOOL WINAPI GetUserNameW(LPWSTR lpBuffer, LPDWORD pcbBuffer)
{
#define ERROR_CHECK(hr) do { if (FAILED(hr)) { SetLastError(WIN32_FROM_HRESULT(hr)); return FALSE; } } while (false)

	ComPtr<IUserInformationStatics> info;
	auto hr = RoGetActivationFactory(HStringReference(RuntimeClass_Windows_System_UserProfile_UserInformation).Get(), __uuidof(info), &info);
	ERROR_CHECK(hr);

	boolean isAccessAllowed;
	hr = info->get_NameAccessAllowed(&isAccessAllowed);
	ERROR_CHECK(hr);

	if (!isAccessAllowed)
	{
		SetLastError(ERROR_ACCESS_DENIED);
		return FALSE;
	}

	ComPtr<IAsyncOperation<HSTRING>> op;
	hr = info->GetDisplayNameAsync(&op);
	ERROR_CHECK(hr);

	HString name;
	hr = SynchronousOperation<HSTRING>(op.Get()).GetResults(name.GetAddressOf());
	ERROR_CHECK(hr);

#undef ERROR_CHECK

	return CopyHStringToBuffer(name, lpBuffer, pcbBuffer);
}

BOOL WINAPI GetVersionExW(LPOSVERSIONINFOW lpVersionInformation)
{
	Assert(lpVersionInformation->dwOSVersionInfoSize == sizeof(OSVERSIONINFOW));

#define ERROR_CHECK(hr) do { if (FAILED(hr)) { SetLastError(WIN32_FROM_HRESULT(hr)); return FALSE; } } while (false)

	ComPtr<IAnalyticsInfoStatics> analytics;
	auto hr = RoGetActivationFactory(HStringReference(RuntimeClass_Windows_System_Profile_AnalyticsInfo).Get(), __uuidof(IAnalyticsInfoStatics), &analytics);
	ERROR_CHECK(hr);

	ComPtr<IAnalyticsVersionInfo> versionInfo;
	hr = analytics->get_VersionInfo(&versionInfo);
	ERROR_CHECK(hr);

	HString versionString;
	hr = versionInfo->get_DeviceFamilyVersion(versionString.GetAddressOf());
	ERROR_CHECK(hr);

#undef ERROR_CHECK

	unsigned int dummy;
	int64_t versionNumber = _wtoi64(versionString.GetRawBuffer(&dummy));

	if (versionNumber == 0)
	{
		SetLastError(ERROR_VERSION_PARSE_ERROR);
		return FALSE;
	}

	lpVersionInformation->dwMajorVersion = versionNumber >> 48;
	lpVersionInformation->dwMinorVersion = (versionNumber >> 32) & 0xFFFF;
	lpVersionInformation->dwBuildNumber = (versionNumber >> 16) & 0xFFFF;
	lpVersionInformation->dwPlatformId = VER_PLATFORM_WIN32_NT;
	ZeroMemory(lpVersionInformation->szCSDVersion, sizeof(lpVersionInformation->szCSDVersion));

	return TRUE;
}

BOOL WINAPI SetEnvironmentVariableW(LPCWSTR lpName, LPCWSTR lpValue)
{
	il2cpp::os::FastAutoLock lock(&s_EnvironmentVariablesMutex);
	
	if (lpValue != NULL)
	{
		s_EnvironmentVariables[lpName] = lpValue;
	}
	else
	{
		s_EnvironmentVariables.erase(lpName);
	}

	return TRUE;
}

} // extern "C"

#endif
