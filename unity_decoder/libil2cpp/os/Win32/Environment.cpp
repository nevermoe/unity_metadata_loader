#include "il2cpp-config.h"

#if !IL2CPP_USE_GENERIC_ENVIRONMENT && IL2CPP_TARGET_WINDOWS
#include "WindowsHelpers.h"
#if !IL2CPP_TARGET_XBOXONE
#include <Shlobj.h>
#endif
// Windows.h defines GetEnvironmentVariable as GetEnvironmentVariableW for unicode and this will
// change the string "Environment::GetEnvironmentVariable" below to "Environment::GetEnvironmentVariableW"
// in the preprocessor. So we undef to avoid this issue and use GetEnvironmentVariableW directly.
// Same for SetEnvironmentVariable
#undef GetEnvironmentVariable
#undef SetEnvironmentVariable
#include "os/Environment.h"
#include "utils/StringUtils.h"
#include <string>
#include <assert.h>

#define BUFFER_SIZE 1024

namespace il2cpp
{
namespace os
{
    std::string Environment::GetMachineName()
    {
        Il2CppChar computerName[MAX_COMPUTERNAME_LENGTH + 1];
        DWORD size = sizeof(computerName) / sizeof(computerName[0]);
        if (!GetComputerNameW(computerName, &size))
            return NULL;

        return utils::StringUtils::Utf16ToUtf8(computerName);
    }

    int32_t Environment::GetProcessorCount()
    {
        SYSTEM_INFO info;
        GetSystemInfo(&info);
        return info.dwNumberOfProcessors;
    }

// GetVersionEx is deprecated on desktop in Windows SDK, and we shim it for WinRT
#pragma warning( push )
#pragma warning( disable : 4996 )

    std::string Environment::GetOsVersionString()
    {
        OSVERSIONINFO verinfo;

        verinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        if (GetVersionEx(&verinfo))
        {
            char version[64];
            /* maximum string length is 35 bytes
               3 x 10 bytes per number, 1 byte for 0, 3 x 1 byte for dots, 1 for NULL */
            sprintf(version, "%ld.%ld.%ld.0",
                verinfo.dwMajorVersion,
                verinfo.dwMinorVersion,
                verinfo.dwBuildNumber);
            return version;
        }

        return "0.0.0.0";
    }

#pragma warning( pop )

    std::string Environment::GetOsUserName()
    {
        Il2CppChar user_name[256 + 1];
        DWORD user_name_size = ARRAYSIZE(user_name);
        if (GetUserNameW(user_name, &user_name_size))
            return utils::StringUtils::Utf16ToUtf8(user_name);

        return "Unknown";
    }

    std::string Environment::GetEnvironmentVariable(const std::string& name)
    {
        Il2CppChar buffer[BUFFER_SIZE];

        const UTF16String varName = utils::StringUtils::Utf8ToUtf16(name.c_str());

        DWORD ret = GetEnvironmentVariableW(varName.c_str(), buffer, BUFFER_SIZE);

        if (ret == 0) // Not found
            return std::string();

        if (ret < BUFFER_SIZE) // Found and fits into buffer
            return utils::StringUtils::Utf16ToUtf8(buffer);

        // Requires bigger buffer
        IL2CPP_ASSERT(ret >= BUFFER_SIZE);

        Il2CppChar* bigbuffer = new Il2CppChar[ret + 1];

        ret = GetEnvironmentVariableW(varName.c_str(), bigbuffer, ret + 1);
        IL2CPP_ASSERT(ret != 0);

        std::string variableValue(utils::StringUtils::Utf16ToUtf8(bigbuffer));

        delete bigbuffer;

        return variableValue;
    }

    void Environment::SetEnvironmentVariable(const std::string& name, const std::string& value)
    {
        const UTF16String varName = utils::StringUtils::Utf8ToUtf16(name.c_str());

        if (value.empty())
            SetEnvironmentVariableW((LPWSTR)varName.c_str(), NULL);
        else
        {
            const UTF16String varValue = utils::StringUtils::Utf8ToUtf16(value.c_str());
            SetEnvironmentVariableW((LPWSTR)varName.c_str(), (LPWSTR)varValue.c_str());
        }
    }

    std::vector<std::string> Environment::GetEnvironmentVariableNames()
    {
        WCHAR* env_strings;
        WCHAR* env_string;
        WCHAR* equal_str;

        std::vector<std::string> result;

        env_strings = GetEnvironmentStringsW();

        if (env_strings)
        {
            env_string = env_strings;
            while (*env_string != '\0')
            {
                // Skip over environment variables starting with '='
                if (*env_string != '=')
                {
                    equal_str = wcschr(env_string, '=');
                    result.push_back(utils::StringUtils::Utf16ToUtf8(env_string, (int)(equal_str - env_string)));
                }
                while (*env_string != '\0')
                    env_string++;
                env_string++;
            }

            FreeEnvironmentStringsW(env_strings);
        }

        return result;
    }

    std::string Environment::GetHomeDirectory()
    {
        NOT_IMPLEMENTED_ICALL(Environment::GetHomeDirectory);
        return std::string();
    }

    std::vector<std::string> Environment::GetLogicalDrives()
    {
        NOT_IMPLEMENTED_ICALL(Environment::GetLogicalDrives);
        return std::vector<std::string>();
    }

    void Environment::Exit(int result)
    {
        NOT_IMPLEMENTED_ICALL(Environment::Exit);
    }

    NORETURN void Environment::Abort()
    {
        // __fastfail() is available since VS2012
#if _MSC_VER >= 1700
        __fastfail(FAST_FAIL_FATAL_APP_EXIT);
#else
        abort();
#endif
    }

#if IL2CPP_TARGET_WINDOWS_DESKTOP

    std::string Environment::GetWindowsFolderPath(int32_t folder)
    {
        Il2CppChar path[MAX_PATH];
        if (SUCCEEDED(SHGetFolderPathW(NULL, folder | CSIDL_FLAG_CREATE, NULL, 0, path)))
            return utils::StringUtils::Utf16ToUtf8(path);

        return std::string();
    }

#if NET_4_0

    typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

    bool Environment::Is64BitOs()
    {
        BOOL isWow64Process = false;

        // Supported on XP SP2 and higher

        //IsWow64Process is not available on all supported versions of Windows.
        //Use GetModuleHandle to get a handle to the DLL that contains the function
        //and GetProcAddress to get a pointer to the function if available.

        LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(
                GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

        if (NULL != fnIsWow64Process)
        {
            if (fnIsWow64Process(GetCurrentProcess(), &isWow64Process))
            {
                return isWow64Process == TRUE;
            }
        }

        return false;
    }

#endif

#endif
}
}
#endif
