#include "il2cpp-config.h"
#include <cassert>
#include <string>
#include "object-internals.h"
#include "class-internals.h"
#include "icalls/mscorlib/System/MonoEnumInfo.h"
#include "vm/Array.h"
#include "vm/Class.h"
#include "vm/Field.h"
#include "vm/GenericClass.h"
#include "vm/String.h"
#include "vm/Reflection.h"
#include "vm/Type.h"
#include "utils/MemoryRead.h"

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

void MonoEnumInfo::get_enum_info(Il2CppReflectionType* type,Il2CppEnumInfo* info)
{
	Il2CppClass* enumType = Class::FromIl2CppType(type->type);
	
	size_t nvalues = Class::GetNumFields (enumType) ? Class::GetNumFields (enumType) - 1 : 0;
	
	IL2CPP_OBJECT_SETREF (info, utype, Reflection::GetTypeObject(Type::GetUnderlyingType(type->type)));
	IL2CPP_OBJECT_SETREF (info, names, (Il2CppArray*)il2cpp::vm::Array::New (il2cpp_defaults.string_class, (il2cpp_array_size_t)nvalues));
	IL2CPP_OBJECT_SETREF (info, values, (Il2CppArray*)il2cpp::vm::Array::New (enumType, (il2cpp_array_size_t)nvalues));
	
	if(enumType->generic_class)
		enumType = GenericClass::GetTypeDefinition (enumType->generic_class);
	
	FieldInfo* field;
	void* iter = NULL;

	int j = 0;
	while ((field = Class::GetFields (enumType, &iter))) {
		const char *p;
		const Il2CppType* type = NULL;

		if (strcmp ("value__", field->name) == 0)
			continue;
		if (Field::IsDeleted (field))
			continue;
		il2cpp_array_setref (info->names, j, il2cpp::vm::String::New (Field::GetName (field)));

		p = Class::GetFieldDefaultValue (field, &type);
		switch (Class::GetEnumBaseType (enumType)->type) {
		case IL2CPP_TYPE_U1:
		case IL2CPP_TYPE_I1:
			il2cpp_array_set (info->values, uint8_t, j, *p);
			break;
		case IL2CPP_TYPE_CHAR:
			il2cpp_array_set(info->values, Il2CppChar, j, ReadChar(p));
			break;
		case IL2CPP_TYPE_U2:
		case IL2CPP_TYPE_I2:
			il2cpp_array_set (info->values, uint16_t, j, Read16 (p));
			break;
		case IL2CPP_TYPE_U4:
		case IL2CPP_TYPE_I4:
			il2cpp_array_set (info->values, uint32_t, j, Read32 (p));
			break;
		case IL2CPP_TYPE_U8:
		case IL2CPP_TYPE_I8:
			il2cpp_array_set (info->values, uint64_t, j, Read64 (p));
			break;
		default:
			assert (0);
		}
		j++;
	}
}

} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
