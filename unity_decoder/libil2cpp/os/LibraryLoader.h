#pragma once

#include "il2cpp-config.h"
#include "utils/StringView.h"
#include <string>

struct PInvokeArguments;

namespace il2cpp
{
namespace os
{

class LibraryLoader
{
public:
	static void* LoadDynamicLibrary(const utils::StringView<Il2CppNativeChar>& nativeDynamicLibrary);
	static Il2CppMethodPointer GetFunctionPointer(void* dynamicLibrary, const PInvokeArguments& pinvokeArgs);
	static Il2CppMethodPointer GetFunctionPointer(void* dynamicLibrary, const char* functionName);
	static void CleanupLoadedLibraries();
};

} /* namespace os */
} /* namespace il2cpp*/
