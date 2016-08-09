#include "il2cpp-config.h"

#if IL2CPP_TARGET_POSIX

//#define VERBOSE_OUTPUT

#ifdef VERBOSE_OUTPUT
#include <stdio.h>
#endif

#include <dlfcn.h>
#include <string>
#include <set>
#include <cassert>

#include "metadata.h"
#include "os/LibraryLoader.h"
#include "utils/StringUtils.h"
#include "vm/Exception.h"
#include "vm/PlatformInvoke.h"

namespace il2cpp
{
namespace os
{

static std::set<void*> s_NativeHandlesOpen;
typedef std::set<void*>::const_iterator OpenHandleIterator;

struct LibraryNamePrefixAndSuffix
{
	LibraryNamePrefixAndSuffix(const char* prefix_, const char* suffix_)
	{
		prefix = std::string(prefix_);
		suffix = std::string(suffix_);
	}

	std::string prefix;
	std::string suffix;
};

static LibraryNamePrefixAndSuffix LibraryNamePrefixAndSuffixVariations[8] = {
	LibraryNamePrefixAndSuffix("", ".so"),
	LibraryNamePrefixAndSuffix("", ".dll"),
	LibraryNamePrefixAndSuffix("", ".dylib"),
	LibraryNamePrefixAndSuffix("", ".bundle"),
	LibraryNamePrefixAndSuffix("lib", ".so"),
	LibraryNamePrefixAndSuffix("lib", ".dll"),
	LibraryNamePrefixAndSuffix("lib", ".dylib"),
	LibraryNamePrefixAndSuffix("lib", ".bundle")
};

// Note that testing this code can be a bit difficult, since dlopen will cache
// the values it returns, and we don't call dlcose from the C# level. See the
// comments in the integration test code for more details.

static void* LoadLibraryWithName(const char* name)
{
#ifdef VERBOSE_OUTPUT
	printf("Trying name: %s\n", name.c_str());
#endif
	void* handle = dlopen(name, RTLD_LAZY);
	if (handle != NULL)
		return handle;

#ifdef VERBOSE_OUTPUT
	printf("Error: %s\n", dlerror());
#endif

	return NULL;
}

static void* CheckLibraryVariations(const char* name)
{
	int numberOfVariations = sizeof(LibraryNamePrefixAndSuffixVariations) / sizeof(LibraryNamePrefixAndSuffixVariations[0]);
	for (int i = 0; i < numberOfVariations; ++i)
	{
		std::string libraryName = LibraryNamePrefixAndSuffixVariations[i].prefix + name + LibraryNamePrefixAndSuffixVariations[i].suffix;
		void* handle = LoadLibraryWithName(libraryName.c_str());
		if (handle != NULL)
			return handle;
	}

	return NULL;
}

void* LibraryLoader::LoadDynamicLibrary(const utils::StringView<Il2CppNativeChar>& nativeDynamicLibrary)
{
#ifdef VERBOSE_OUTPUT
	printf("Attempting to load dynamic library: %s\n", nativeDynamicLibrary.c_str());
#endif

	StringViewAsNullTerminatedStringOf(char, nativeDynamicLibrary, libraryName);
	void* handle = LoadLibraryWithName(libraryName);

	if (handle == NULL)
		handle = CheckLibraryVariations(libraryName);

	if (handle == NULL)
	{
		size_t lengthWithoutDotDll = nativeDynamicLibrary.Length() - 4;
		if (strncmp(libraryName + lengthWithoutDotDll, ".dll", 4) == 0)
		{
			char* nativeDynamicLibraryWithoutExtension = static_cast<char*>(alloca((lengthWithoutDotDll + 1) * sizeof(char)));
			memcpy(nativeDynamicLibraryWithoutExtension, libraryName, lengthWithoutDotDll);
			nativeDynamicLibraryWithoutExtension[lengthWithoutDotDll] = 0;

			handle = CheckLibraryVariations(nativeDynamicLibraryWithoutExtension);
		}
	}

	if (handle != NULL)
		s_NativeHandlesOpen.insert(handle);

	return handle;
}

Il2CppMethodPointer LibraryLoader::GetFunctionPointer(void* dynamicLibrary, const PInvokeArguments& pinvokeArgs)
{
	StringViewAsNullTerminatedStringOf(char, pinvokeArgs.entryPoint, entryPoint);
	return GetFunctionPointer(dynamicLibrary, entryPoint);
}

Il2CppMethodPointer LibraryLoader::GetFunctionPointer(void* dynamicLibrary, const char* functionName)
{
#ifdef VERBOSE_OUTPUT
	printf("Attempting to load method at entry point: %s\n", pinvokeArgs.entryPoint);
#endif

	Il2CppMethodPointer method = reinterpret_cast<Il2CppMethodPointer>(dlsym(dynamicLibrary, functionName));

#ifdef VERBOSE_OUTPUT
	if (method == NULL)
		printf("Error: %s\n", dlerror());
#endif

	return method;
}

void LibraryLoader::CleanupLoadedLibraries()
{
	for (OpenHandleIterator it = s_NativeHandlesOpen.begin(); it != s_NativeHandlesOpen.end(); it++)
	{
		dlclose(*it);
	}
}

}
}

#endif
