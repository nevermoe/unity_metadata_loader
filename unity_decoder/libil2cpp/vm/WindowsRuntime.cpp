#include "il2cpp-config.h"
#include "metadata/GenericMetadata.h"
#include "os/LibraryLoader.h"
#include "os/WindowsRuntime.h"
#include "utils/Il2CppHStringReference.h"
#include "utils/StringUtils.h"
#include "vm/Class.h"
#include "vm/Exception.h"
#include "vm/GenericClass.h"
#include "vm/MetadataCache.h"
#include "vm/WindowsRuntime.h"

using namespace il2cpp::metadata;

namespace il2cpp
{
namespace vm
{
    Il2CppIActivationFactory* WindowsRuntime::GetActivationFactory(const utils::StringView<Il2CppNativeChar>& runtimeClassName)
    {
        utils::Il2CppHStringReference className(runtimeClassName);
        Il2CppIActivationFactory* factory = NULL;
        il2cpp_hresult_t hr = os::WindowsRuntime::GetActivationFactory(className, &factory);

        if (IL2CPP_HR_SUCCEEDED(hr))
            return factory;

        if (hr != IL2CPP_REGDB_E_CLASSNOTREG)
            Exception::Raise(hr, false);

        // If GetActivationFactory doesn't find the class, we can still try to find it manually
        // All Windows runtime classes must be in namespaces, and that class has to be in a DLL
        // that is named after the namespace of a part of it.
        // For example, MyNamespace.MySubNamespace.MyClass can be in either
        // MyNamespace.MySubNamespace.dll or MyNamespace.dll
        IL2CPP_ASSERT(runtimeClassName.Length() > 1);
        size_t namespaceEnd = runtimeClassName.Length() - 1;

        do
        {
            namespaceEnd--;
        }
        while (namespaceEnd > 0 && runtimeClassName[namespaceEnd] != '.');

        Il2CppNativeChar* nativeDll = static_cast<Il2CppNativeChar*>(alloca((namespaceEnd + 5) * sizeof(Il2CppNativeChar)));
        memcpy(nativeDll, runtimeClassName.Str(), namespaceEnd * sizeof(Il2CppNativeChar));

        while (namespaceEnd > 0)
        {
            memcpy(nativeDll + namespaceEnd, IL2CPP_NATIVE_STRING(".dll"), 4 * sizeof(Il2CppNativeChar));
            nativeDll[namespaceEnd + 4] = 0;

            void* dynamicLibrary = os::LibraryLoader::LoadDynamicLibrary(utils::StringView<Il2CppNativeChar>(nativeDll, namespaceEnd + 4));
            if (dynamicLibrary != NULL)
            {
                typedef il2cpp_hresult_t(STDCALL * DllGetActivationFactory)(Il2CppHString activatableClassId, Il2CppIActivationFactory** factory);
                DllGetActivationFactory dllGetActivationFactory = reinterpret_cast<DllGetActivationFactory>(os::LibraryLoader::GetFunctionPointer(dynamicLibrary, "DllGetActivationFactory"));

                if (dllGetActivationFactory != NULL)
                {
                    hr = dllGetActivationFactory(className, &factory);

                    if (IL2CPP_HR_SUCCEEDED(hr))
                        return factory;

                    if (hr != IL2CPP_REGDB_E_CLASSNOTREG)
                        Exception::Raise(hr, false);
                }
            }

            do
            {
                namespaceEnd--;
            }
            while (namespaceEnd > 0 && runtimeClassName[namespaceEnd] != '.');
        }

        Exception::Raise(IL2CPP_REGDB_E_CLASSNOTREG, false);

        return NULL;
    }
}
}
