#include "MarshalingUtils.h"

namespace il2cpp
{
namespace utils
{
    void MarshalingUtils::MarshalStructToNative(void* managedStructure, void* marshaledStructure, const Il2CppInteropData* interopData)
    {
        IL2CPP_ASSERT(interopData);
        IL2CPP_ASSERT(interopData->pinvokeMarshalToNativeFunction);
        interopData->pinvokeMarshalToNativeFunction(managedStructure, marshaledStructure);
    }

    void MarshalingUtils::MarshalStructFromNative(void* marshaledStructure, void* managedStructure, const Il2CppInteropData* interopData)
    {
        IL2CPP_ASSERT(interopData);
        IL2CPP_ASSERT(interopData->pinvokeMarshalFromNativeFunction);
        interopData->pinvokeMarshalFromNativeFunction(marshaledStructure, managedStructure);
    }

    bool MarshalingUtils::MarshalFreeStruct(void* marshaledStructure, const Il2CppInteropData* interopData)
    {
        if (interopData == NULL)
            return false;

        PInvokeMarshalCleanupFunc cleanup = interopData->pinvokeMarshalCleanupFunction;

        if (cleanup == NULL)
            return false;

        cleanup(marshaledStructure);
        return true;
    }
} // namespace utils
} // namespace il2cpp
