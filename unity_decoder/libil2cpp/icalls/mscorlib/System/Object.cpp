#include "il2cpp-config.h"
#include "object-internals.h"
#include "class-internals.h"
#include "icalls/mscorlib/System/Object.h"
#include "vm/Object.h"
#include "vm/Reflection.h"
#include "vm/Exception.h"

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{
    int Object::InternalGetHashCode(Il2CppObject* obj)
    {
        return il2cpp::vm::Object::GetHash(obj);
    }

    Il2CppReflectionType* Object::GetType(Il2CppObject* obj)
    {
        return il2cpp::vm::Reflection::GetTypeObject(obj->klass->byval_arg);
    }

    Il2CppObject* Object::MemberwiseClone(Il2CppObject* obj)
    {
        return il2cpp::vm::Object::Clone(obj);
    }

    Il2CppIntPtr Object::obj_address(Il2CppObject* obj)
    {
        Il2CppIntPtr addr = { obj };
        return addr;
    }
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
