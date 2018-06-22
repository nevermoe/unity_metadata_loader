#if NET_4_0
#include "il2cpp-config.h"
#include "RuntimeFieldHandle.h"
#include "icalls/mscorlib/System.Reflection/MonoField.h"

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{
    void RuntimeFieldHandle::SetValueDirect(Il2CppObject* field, Il2CppObject* fieldType, void* pTypedRef, Il2CppObject* value, Il2CppObject* contextType)
    {
        NOT_IMPLEMENTED_ICALL(RuntimeFieldHandle::SetValueDirect);
        IL2CPP_UNREACHABLE;
    }

    void RuntimeFieldHandle::SetValueInternal(Il2CppReflectionField* fi, Il2CppObject* obj, Il2CppObject* value)
    {
        // In mono's icall-def.h file, this maps to the same icall as MonoField.SetValueInternal
        // so our implementation will do the same
        Reflection::MonoField::SetValueInternal(fi, obj, value);
    }
} // namespace System
} // namespace mscorlib
} // namespace icalls
} // namespace il2cpp
#endif
