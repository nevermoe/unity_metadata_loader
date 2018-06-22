#include "il2cpp-config.h"

#include "icalls/mscorlib/System/TypedReference.h"
#include "vm/Exception.h"

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{
    Il2CppObject* TypedReference::ToObject(void* /* System.TypedReference */ value)
    {
        NOT_SUPPORTED_IL2CPP(TypedReference::ToObject, "This icall is not supported by il2cpp.");

        return 0;
    }

#if NET_4_0
    Il2CppObject* TypedReference::InternalToObject(void* value)
    {
        NOT_IMPLEMENTED_ICALL(TypedReference::InternalToObject);
        IL2CPP_UNREACHABLE;
        return NULL;
    }

    Il2CppTypedRef TypedReference::MakeTypedReferenceInternal(Il2CppObject* target, Il2CppArray* fields)
    {
        NOT_IMPLEMENTED_ICALL(TypedReference::MakeTypedReferenceInternal);
        IL2CPP_UNREACHABLE;
        return Il2CppTypedRef();
    }

#endif
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
