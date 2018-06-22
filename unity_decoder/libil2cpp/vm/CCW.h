#pragma once

#include "gc/GarbageCollector.h"

struct Il2CppIUnknown;
struct Il2CppObject;
struct Il2CppException;

namespace il2cpp
{
namespace vm
{
    class LIBIL2CPP_CODEGEN_API CCW
    {
    public:
        // CreateCCW returns upcasted Il2CppIManagedObjectHolder if the CCW is cachable!
        static Il2CppIUnknown* CreateCCW(Il2CppObject* obj);

        static inline Il2CppIUnknown* GetOrCreate(Il2CppObject* obj, const Il2CppGuid& iid)
        {
            return gc::GarbageCollector::GetOrCreateCCW(obj, iid);
        }

        static Il2CppException* GetIPropertyValueInvalidCast(Il2CppObject* value, const char* from, const char* to);
    };
} /* namespace vm */
} /* namespace il2cpp */
