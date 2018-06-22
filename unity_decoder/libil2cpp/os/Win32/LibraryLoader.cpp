#include "il2cpp-config.h"

#if IL2CPP_TARGET_WINDOWS || IL2CPP_TARGET_XBOXONE

#include "metadata.h"
#include "os/Mutex.h"
#include "os/LibraryLoader.h"
#include "vm/PlatformInvoke.h"
#include "utils/StringUtils.h"

#include <sstream>
#include <unordered_map>

#include "WindowsHelpers.h"

namespace il2cpp
{
namespace os
{

static std::vector<std::pair<std::wstring, HMODULE>> s_NativeDllCache;
typedef std::vector<std::pair<std::wstring, HMODULE>>::const_iterator DllCacheIterator;
os::FastMutex s_NativeDllCacheMutex;

void* LibraryLoader::LoadDynamicLibrary(const utils::StringView<Il2CppNativeChar>& nativeDynamicLibrary)
{
	{
		os::FastAutoLock lock(&s_NativeDllCacheMutex);

		for (DllCacheIterator it = s_NativeDllCache.begin(); it != s_NativeDllCache.end(); it++)
		{
			if (it->first.compare(0, std::string::npos, nativeDynamicLibrary.Str(), nativeDynamicLibrary.Length()) == 0)
				return it->second;
		}
	}

	std::wstring dllName(nativeDynamicLibrary.Str(), nativeDynamicLibrary.Length());
	HMODULE module = ::LoadLibraryW(dllName.c_str());

	if (module != NULL)
	{
		os::FastAutoLock lock(&s_NativeDllCacheMutex);
		s_NativeDllCache.push_back(std::make_pair(std::move(dllName), module));
	}

	return module;
}

Il2CppMethodPointer LibraryLoader::GetFunctionPointer(void* dynamicLibrary, const PInvokeArguments& pinvokeArgs)
{
	if (dynamicLibrary == NULL)
		return NULL;

	HMODULE module = (HMODULE)dynamicLibrary;
	StringViewAsNullTerminatedStringOf(char, pinvokeArgs.entryPoint, entryPoint);

	// If there's 'no mangle' flag set, just return directly what GetProcAddress returns
	if (pinvokeArgs.isNoMangle)
		return reinterpret_cast<Il2CppMethodPointer>(GetProcAddress(module, entryPoint));

	const size_t kBufferOverhead = 10;
	FARPROC functionPtr = NULL;
	size_t originalFuncNameLength = strlen(entryPoint) + 1;
	std::string functionName;

	functionName.resize(originalFuncNameLength + kBufferOverhead + 1);	// Let's index the string from '1', because we might have to prepend an underscore in case of stdcall mangling
	memcpy(&functionName[1], entryPoint, originalFuncNameLength);
	ZeroMemory(&functionName[1] + originalFuncNameLength, kBufferOverhead);

	// If there's no 'dont mangle' flag set, 'W' function takes priority over original name, but 'A' function does not (yes, really)
	if (pinvokeArgs.charSet == CHARSET_UNICODE)
	{
		functionName[originalFuncNameLength] = 'W';
		functionPtr = GetProcAddress(module, functionName.c_str() + 1);
		if (functionPtr != NULL)
			return reinterpret_cast<Il2CppMethodPointer>(functionPtr);

		// If charset specific function lookup failed, try with original name
		functionPtr = GetProcAddress(module, entryPoint);
	}
	else
	{
		functionPtr = GetProcAddress(module, entryPoint);
		if (functionPtr != NULL)
			return reinterpret_cast<Il2CppMethodPointer>(functionPtr);

		// If original name function lookup failed, try with mangled name
		functionName[originalFuncNameLength] = 'A';
		functionPtr = GetProcAddress(module, functionName.c_str() + 1);
	}

	if (functionPtr != NULL)
		return reinterpret_cast<Il2CppMethodPointer>(functionPtr);

	// If it's not cdecl, try mangling the name
	// THIS ONLY APPLIES TO 32-bit x86!
#if !defined(__arm__)
	if (sizeof(void*) == 4 && pinvokeArgs.callingConvention != IL2CPP_CALL_C)
	{
		functionName[0] = '_';
		sprintf(&functionName[0] + originalFuncNameLength, "@%ld", pinvokeArgs.parameterSize);

		functionPtr = GetProcAddress(module, functionName.c_str());
	}
#endif

	return reinterpret_cast<Il2CppMethodPointer>(functionPtr);
}

Il2CppMethodPointer LibraryLoader::GetFunctionPointer(void* dynamicLibrary, const char* functionName)
{
	return reinterpret_cast<Il2CppMethodPointer>(GetProcAddress(static_cast<HMODULE>(dynamicLibrary), functionName));
}

void LibraryLoader::CleanupLoadedLibraries()
{
	for (DllCacheIterator it = s_NativeDllCache.begin(); it != s_NativeDllCache.end(); it++)
	{
		FreeLibrary(it->second);
	}
}

}
}

#endif
