#include "il2cpp-config.h"
#include <string>
#include "object-internals.h"
#include "class-internals.h"
#include "icalls/mscorlib/System/MonoEnumInfo.h"
#include "vm/Array.h"
#include "vm/Class.h"
#include "vm/Enum.h"
#include "vm/Field.h"
#include "vm/GenericClass.h"
#include "vm/String.h"
#include "vm/Reflection.h"
#include "vm/Type.h"
#include "utils/MemoryRead.h"

#if !NET_4_0

using namespace il2cpp::vm;
using namespace il2cpp::utils;

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{
    void MonoEnumInfo::get_enum_info(Il2CppReflectionType* type, Il2CppEnumInfo* info)
    {
        IL2CPP_OBJECT_SETREF(info, utype, Reflection::GetTypeObject(Type::GetUnderlyingType(type->type)));
        vm::Enum::GetEnumValuesAndNames(Class::FromIl2CppType(type->type), &info->values, &info->names);
    }
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */

#endif
