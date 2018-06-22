#include "il2cpp-config.h"
#include "os/WindowsRuntime.h"
#include "WindowsHeaders.h"

#if IL2CPP_PLATFORM_WIN32

namespace il2cpp
{

namespace os
{

#if !LINK_TO_WINDOWSRUNTIME_LIBS
template <typename FunctionType>
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
	assert(className != NULL);
	assert(activationFactory != NULL);

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
	assert(header != NULL);
	assert(hstring != NULL);

#if LINK_TO_WINDOWSRUNTIME_LIBS
	return WindowsCreateStringReference(str.Str(), static_cast<uint32_t>(str.Length()), reinterpret_cast<HSTRING_HEADER*>(header), reinterpret_cast<HSTRING*>(hstring));
#else
	typedef HRESULT(STDAPICALLTYPE* WindowsCreateStringReferenceFunc)(const wchar_t* sourceString, uint32_t length, Il2CppHStringHeader* hstringHeader, Il2CppHString* hstring);
	static WindowsCreateStringReferenceFunc WindowsCreateStringReference = NULL;

	if (WindowsCreateStringReference == NULL)
	{
		WindowsCreateStringReference = ResolveAPI<WindowsCreateStringReferenceFunc>(L"api-ms-win-core-winrt-string-l1-1-0.dll", "WindowsCreateStringReference");

		if (WindowsCreateStringReference == NULL)
			return IL2CPP_E_UNEXPECTED;
	}

	return WindowsCreateStringReference(str.Str(), static_cast<uint32_t>(str.Length()), header, hstring);
#endif
}


}
}

#endif