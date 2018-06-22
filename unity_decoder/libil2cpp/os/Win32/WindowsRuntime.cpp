#include "il2cpp-config.h"
#include "class-internals.h"
#include "il2cpp-string-types.h"
#include "il2cpp-vm-support.h"
#include "os/WindowsRuntime.h"
#include "utils/Il2CppHStringReference.h"
#include "utils/StringUtils.h"
#include "vm/Array.h"
#include "vm/CCW.h"
#include "vm/Exception.h"
#include "vm/String.h"
#include "WindowsHeaders.h"

#if IL2CPP_TARGET_WINDOWS && !IL2CPP_USE_GENERIC_WINDOWSRUNTIME

#if LINK_TO_WINDOWSRUNTIME_LIBS
#include <roerrorapi.h>
#else

typedef enum
{
    RO_ERROR_REPORTING_NONE = 0x00000000,
    RO_ERROR_REPORTING_SUPPRESSEXCEPTIONS = 0x00000001,
    RO_ERROR_REPORTING_FORCEEXCEPTIONS = 0x00000002,
    RO_ERROR_REPORTING_USESETERRORINFO = 0x00000004,
    RO_ERROR_REPORTING_SUPPRESSSETERRORINFO = 0x00000008,
} RO_ERROR_REPORTING_FLAGS;

#endif

namespace il2cpp
{
namespace os
{
#if !LINK_TO_WINDOWSRUNTIME_LIBS
    template<typename FunctionType>
    FunctionType ResolveAPI(const wchar_t* moduleName, const char* functionName)
    {
        HMODULE module = GetModuleHandleW(moduleName);

        if (module == NULL)
            return NULL;

        return reinterpret_cast<FunctionType>(GetProcAddress(module, functionName));
    }

#endif

    il2cpp_hresult_t WindowsRuntime::GetActivationFactory(Il2CppHString className, Il2CppIActivationFactory** activationFactory)
    {
        IL2CPP_ASSERT(className != NULL);
        IL2CPP_ASSERT(activationFactory != NULL);

#if LINK_TO_WINDOWSRUNTIME_LIBS
        return RoGetActivationFactory(reinterpret_cast<HSTRING>(className), reinterpret_cast<const IID&>(Il2CppIActivationFactory::IID), reinterpret_cast<void**>(activationFactory));
#else
        typedef HRESULT(WINAPI* RoGetActivationFactoryFunc)(void* activatableClassId, const Il2CppGuid& iid, Il2CppIActivationFactory** factory);
        static RoGetActivationFactoryFunc RoGetActivationFactory = NULL;

        if (RoGetActivationFactory == NULL)
        {
            RoGetActivationFactory = ResolveAPI<RoGetActivationFactoryFunc>(L"api-ms-win-core-winrt-l1-1-0.dll", "RoGetActivationFactory");

            if (RoGetActivationFactory == NULL)
                return IL2CPP_REGDB_E_CLASSNOTREG;
        }

        return RoGetActivationFactory(className, Il2CppIActivationFactory::IID, activationFactory);
#endif
    }

    il2cpp_hresult_t WindowsRuntime::CreateHStringReference(const utils::StringView<Il2CppNativeChar>& str, Il2CppHStringHeader* header, Il2CppHString* hstring)
    {
        IL2CPP_ASSERT(header != NULL);
        IL2CPP_ASSERT(hstring != NULL);

        if (str.Length() == 0)
        {
            *hstring = NULL;
            return S_OK;
        }

#if LINK_TO_WINDOWSRUNTIME_LIBS
        return WindowsCreateStringReference(str.Str(), static_cast<uint32_t>(str.Length()), reinterpret_cast<HSTRING_HEADER*>(header), reinterpret_cast<HSTRING*>(hstring));
#else
        typedef HRESULT(STDAPICALLTYPE * WindowsCreateStringReferenceFunc)(const wchar_t* sourceString, uint32_t length, Il2CppHStringHeader* hstringHeader, Il2CppHString* hstring);
        static WindowsCreateStringReferenceFunc WindowsCreateStringReference = NULL;

        if (WindowsCreateStringReference == NULL)
        {
            WindowsCreateStringReference = ResolveAPI<WindowsCreateStringReferenceFunc>(L"api-ms-win-core-winrt-string-l1-1-0.dll", "WindowsCreateStringReference");

            if (WindowsCreateStringReference == NULL)
                return IL2CPP_COR_E_PLATFORMNOTSUPPORTED;
        }

        return WindowsCreateStringReference(str.Str(), static_cast<uint32_t>(str.Length()), header, hstring);
#endif
    }

    il2cpp_hresult_t WindowsRuntime::CreateHString(const utils::StringView<Il2CppChar>& str, Il2CppHString* hstring)
    {
        IL2CPP_ASSERT(str.Str() != NULL || str.Length() == 0);

        if (str.Length() == 0)
        {
            *hstring = NULL;
            return S_OK;
        }

#if LINK_TO_WINDOWSRUNTIME_LIBS
        return WindowsCreateString(str.Str(), static_cast<uint32_t>(str.Length()), reinterpret_cast<HSTRING*>(hstring));
#else
        typedef il2cpp_hresult_t (STDAPICALLTYPE * WindowsCreateStringFunc)(const wchar_t* sourceString, uint32_t length, Il2CppHString* hstring);
        static WindowsCreateStringFunc WindowsCreateString = NULL;

        if (WindowsCreateString == NULL)
        {
            WindowsCreateString = ResolveAPI<WindowsCreateStringFunc>(L"api-ms-win-core-winrt-string-l1-1-0.dll", "WindowsCreateString");

            if (WindowsCreateString == NULL)
                return IL2CPP_COR_E_PLATFORMNOTSUPPORTED;
        }

        return WindowsCreateString(str.Str(), static_cast<uint32_t>(str.Length()), hstring);
#endif
    }

    il2cpp_hresult_t WindowsRuntime::DeleteHString(Il2CppHString hstring)
    {
        if (hstring == NULL)
            return IL2CPP_S_OK;

#if LINK_TO_WINDOWSRUNTIME_LIBS
        return WindowsDeleteString(reinterpret_cast<HSTRING>(hstring));
#else
        typedef il2cpp_hresult_t (STDAPICALLTYPE * WindowsDeleteStringFunc)(Il2CppHString hstring);
        static WindowsDeleteStringFunc WindowsDeleteString = NULL;

        if (WindowsDeleteString == NULL)
        {
            WindowsDeleteString = ResolveAPI<WindowsDeleteStringFunc>(L"api-ms-win-core-winrt-string-l1-1-0.dll", "WindowsDeleteString");

            if (WindowsDeleteString == NULL)
                return IL2CPP_COR_E_PLATFORMNOTSUPPORTED;
        }

        return WindowsDeleteString(hstring);
#endif
    }

    const Il2CppChar* WindowsRuntime::GetHStringBuffer(Il2CppHString hstring, uint32_t* length)
    {
#if LINK_TO_WINDOWSRUNTIME_LIBS
        return WindowsGetStringRawBuffer(reinterpret_cast<HSTRING>(hstring), length);
#else
        typedef const wchar_t* (STDAPICALLTYPE * WindowsGetStringRawBufferFunc)(Il2CppHString hstring, uint32_t* length);
        static WindowsGetStringRawBufferFunc WindowsGetStringRawBuffer = NULL;

        if (WindowsGetStringRawBuffer == NULL)
        {
            WindowsGetStringRawBuffer = ResolveAPI<WindowsGetStringRawBufferFunc>(L"api-ms-win-core-winrt-string-l1-1-0.dll", "WindowsGetStringRawBuffer");

            if (WindowsGetStringRawBuffer == NULL)
                IL2CPP_VM_NOT_SUPPORTED(GetHStringBuffer, "Marshaling HSTRINGs is not supported on current platform.");
        }

        return WindowsGetStringRawBuffer(hstring, length);
#endif
    }

    Il2CppString* WindowsRuntime::HStringToManagedString(Il2CppHString hstring)
    {
        if (hstring == NULL)
            return IL2CPP_VM_STRING_EMPTY();

        uint32_t length;
        const wchar_t* ptr = GetHStringBuffer(hstring, &length);
        return IL2CPP_VM_STRING_NEW_UTF16(ptr, length);
    }

    Il2CppIRestrictedErrorInfo* WindowsRuntime::GetRestrictedErrorInfo()
    {
        Il2CppIRestrictedErrorInfo* errorInfo;
        HRESULT hr;

#if LINK_TO_WINDOWSRUNTIME_LIBS
        hr = ::GetRestrictedErrorInfo(reinterpret_cast<IRestrictedErrorInfo**>(&errorInfo));
#else
        typedef HRESULT (STDAPICALLTYPE * GetRestrictedErrorInfoFunc)(Il2CppIRestrictedErrorInfo** ppRestrictedErrorInfo);
        static GetRestrictedErrorInfoFunc getRestrictedErrorInfo = NULL;

        if (getRestrictedErrorInfo == NULL)
        {
            getRestrictedErrorInfo = ResolveAPI<GetRestrictedErrorInfoFunc>(L"api-ms-win-core-winrt-error-l1-1-1.dll", "GetRestrictedErrorInfo");

            if (getRestrictedErrorInfo == NULL)
                return NULL;
        }

        hr = getRestrictedErrorInfo(&errorInfo);
#endif

        if (FAILED(hr))
            return NULL;

        return errorInfo;
    }

// Fallback path for desktop in case we're running on below Windows 8.1
// Also used for Xbox One as it has no RoOriginateLanguageException
    static inline void OriginateErrorNoLanguageException(Il2CppException* ex, Il2CppHString message)
    {
#if !LINK_TO_WINDOWSRUNTIME_LIBS
        typedef BOOL(STDAPICALLTYPE * RoOriginateErrorFunc)(il2cpp_hresult_t error, Il2CppHString message);
        static RoOriginateErrorFunc RoOriginateError = NULL;

        if (RoOriginateError == NULL)
        {
            RoOriginateError = ResolveAPI<RoOriginateErrorFunc>(L"api-ms-win-core-winrt-error-l1-1-0.dll", "RoOriginateError");

            if (RoOriginateError == NULL)
            {
                // We're running on Win7 or below. Give up.
                return;
            }
        }

        RoOriginateError(ex->hresult, message);
#else
        RoOriginateError(ex->hresult, reinterpret_cast<HSTRING>(message));
#endif
    }

#if !IL2CPP_TARGET_XBOXONE

    inline void OriginateLanguageExceptionWithFallback(Il2CppException* ex, Il2CppHString message)
    {
#if !LINK_TO_WINDOWSRUNTIME_LIBS
        typedef BOOL(STDAPICALLTYPE * RoOriginateLanguageExceptionFunc)(il2cpp_hresult_t error, Il2CppHString message, Il2CppIUnknown* languageException);
        static RoOriginateLanguageExceptionFunc RoOriginateLanguageException = NULL;

        if (RoOriginateLanguageException == NULL)
        {
            RoOriginateLanguageException = ResolveAPI<RoOriginateLanguageExceptionFunc>(L"api-ms-win-core-winrt-error-l1-1-1.dll", "RoOriginateLanguageException");

            if (RoOriginateLanguageException == NULL)
            {
                // We're running on Win8 or below. Fallback to RoOriginateError
                OriginateErrorNoLanguageException(ex, message);
                return;
            }
        }
#endif

        Il2CppIUnknown* exceptionCCW = IL2CPP_VM_GET_CREATE_CCW_EXCEPTION(ex);

#if LINK_TO_WINDOWSRUNTIME_LIBS
        RoOriginateLanguageException(ex->hresult, reinterpret_cast<HSTRING>(static_cast<Il2CppHString>(message)), reinterpret_cast<IUnknown*>(exceptionCCW));
#else
        RoOriginateLanguageException(ex->hresult, message, exceptionCCW);
#endif

        exceptionCCW->Release();
    }

#endif // !IL2CPP_TARGET_XBOXONE

    void WindowsRuntime::OriginateLanguageException(Il2CppException* ex, Il2CppString* exceptionString)
    {
        utils::StringView<Il2CppNativeChar> message(utils::StringUtils::GetChars(exceptionString), utils::StringUtils::GetLength(exceptionString));
        utils::Il2CppHStringReference messageHString(message);

#if IL2CPP_TARGET_XBOXONE
        OriginateErrorNoLanguageException(ex, messageHString);
#else
        OriginateLanguageExceptionWithFallback(ex, messageHString);
#endif
    }

    void WindowsRuntime::EnableErrorReporting()
    {
#if !LINK_TO_WINDOWSRUNTIME_LIBS
        typedef il2cpp_hresult_t (STDCALL * RoSetErrorReportingFlagsFunc)(uint32_t flags);
        static RoSetErrorReportingFlagsFunc RoSetErrorReportingFlags = NULL;

        if (RoSetErrorReportingFlags == NULL)
        {
            RoSetErrorReportingFlags = ResolveAPI<RoSetErrorReportingFlagsFunc>(L"api-ms-win-core-winrt-error-l1-1-0.dll", "RoSetErrorReportingFlags");

            // We're running on Win7 or below. Do nothing
            if (RoSetErrorReportingFlags == NULL)
                return;
        }
#endif

        il2cpp_hresult_t hr = RoSetErrorReportingFlags(RO_ERROR_REPORTING_USESETERRORINFO);
        IL2CPP_ASSERT(IL2CPP_HR_SUCCEEDED(hr) && "RoSetErrorReportingFlags failed");
    }
}
}

#endif
