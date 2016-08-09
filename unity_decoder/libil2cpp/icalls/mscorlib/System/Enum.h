#pragma once

#include "il2cpp-config.h"
struct Il2CppObject;
struct Il2CppReflectionType;
struct Il2CppEnumInfo;

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{

class LIBIL2CPP_CODEGEN_API Enum
{
public:
	static Il2CppObject* ToObject (Il2CppReflectionType* enumType, Il2CppObject* value);
	static int compare_value_to (Il2CppObject* __this, Il2CppObject* other);
	static int32_t get_hashcode (Il2CppObject* __this);
	static Il2CppObject* get_value (Il2CppObject* __this);
	static Il2CppReflectionType* get_underlying_type (Il2CppReflectionType* type);
};

} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
