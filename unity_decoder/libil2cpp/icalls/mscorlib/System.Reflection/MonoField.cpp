#include "il2cpp-config.h"
#include <stddef.h>
#include "icalls/mscorlib/System.Reflection/MonoField.h"
#include <cassert>
#include "utils/StringUtils.h"
#include "utils/BlobReader.h"
#include "vm/Class.h"
#include "vm/Field.h"
#include "vm/Object.h"
#include "vm/Reflection.h"
#include "vm/Runtime.h"
#include "vm/Type.h"
#include "vm/Exception.h"
#include "class-internals.h"
#include "tabledefs.h"

using namespace il2cpp::vm;
using il2cpp::utils::StringUtils;

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{
namespace Reflection
{

Il2CppReflectionType * MonoField::GetParentType (Il2CppReflectionField * field, bool declaring)
{
	Il2CppClass *parent;

	parent = declaring ? field->field->parent: field->klass;

	return il2cpp::vm::Reflection::GetTypeObject (parent->byval_arg);
}

int32_t MonoField::GetFieldOffset (Il2CppReflectionField * field)
{
	return field->field->offset - sizeof(Il2CppObject);
}

Il2CppObject* MonoField::GetValueInternal (Il2CppReflectionField * field, Il2CppObject * obj)
{
	return vm::Field::GetValueObject(field->field, obj);
}

void MonoField::SetValueInternal (Il2CppReflectionField* field, Il2CppObject* obj, Il2CppObject* value)
{
	FieldInfo* fieldInfo = field->field;
	Il2CppClass* fieldType = Class::FromIl2CppType(fieldInfo->type);

	if (value != NULL && !Class::IsAssignableFrom(fieldType, value->klass))
	{
		Exception::Raise(Exception::GetArgumentException("value",
			utils::StringUtils::Printf("Object of type '%s' cannot be converted to type '%s'.",
				Type::GetName(value->klass->byval_arg, IL2CPP_TYPE_NAME_FORMAT_FULL_NAME).c_str(),
				Type::GetName(fieldInfo->type, IL2CPP_TYPE_NAME_FORMAT_FULL_NAME).c_str()
			).c_str()));
	}

	uint8_t* fieldAddress;

	if (fieldInfo->type->attrs & FIELD_ATTRIBUTE_STATIC)
	{
		if (fieldInfo->offset == THREAD_STATIC_FIELD_OFFSET)
		{
			NOT_IMPLEMENTED(Field::StaticSetValue);
		}

		Runtime::ClassInit(fieldInfo->parent);
		fieldAddress = static_cast<uint8_t*>(fieldInfo->parent->static_fields) + fieldInfo->offset;
	}
	else
	{
		assert(obj);
		fieldAddress = reinterpret_cast<uint8_t*>(obj) + fieldInfo->offset;
	}

	if (fieldType->valuetype)
	{
		if (!Class::IsNullable(fieldType))
		{
			uint32_t fieldSize = Class::GetInstanceSize(fieldType) - sizeof(Il2CppObject);

			if (value != NULL)
			{
				memcpy(fieldAddress, Object::Unbox(value), fieldSize);
			}
			else
			{
				// Setting value type to null is defined to zero it out
				memset(fieldAddress, 0, fieldSize);
			}
		}
		else
		{
			Il2CppClass* nullableArg = Class::GetNullableArgument(fieldType);
			uint32_t valueSize = Class::GetInstanceSize(nullableArg) - sizeof(Il2CppObject);

			if (value != NULL)
			{
				memcpy(fieldAddress, Object::Unbox(value), valueSize);
				*(fieldAddress + valueSize) = true;
			}
			else
			{
				*(fieldAddress + valueSize) = false;
			}
		}
	}
	else
	{
		memcpy(fieldAddress, &value, sizeof(Il2CppObject*));
	}
}

Il2CppObject* MonoField::GetRawConstantValue (Il2CppReflectionField* field)
{
	FieldInfo* fieldInfo = field->field;

	if (!(fieldInfo->type->attrs & FIELD_ATTRIBUTE_HAS_DEFAULT))
		Exception::Raise (Exception::GetInvalidOperationException (NULL));

	const Il2CppType* type = NULL;
	const char* data = Class::GetFieldDefaultValue (fieldInfo, &type);

	switch (type->type)
	{
		case IL2CPP_TYPE_U1:
		case IL2CPP_TYPE_I1:
		case IL2CPP_TYPE_BOOLEAN:
		case IL2CPP_TYPE_U2:
		case IL2CPP_TYPE_I2:
		case IL2CPP_TYPE_CHAR:
		case IL2CPP_TYPE_U4:
		case IL2CPP_TYPE_I4:
		case IL2CPP_TYPE_R4:
		case IL2CPP_TYPE_U8:
		case IL2CPP_TYPE_I8:
		case IL2CPP_TYPE_R8:
		{
			Il2CppObject* obj = Object::New (Class::FromIl2CppType (type));
			utils::BlobReader::GetConstantValueFromBlob (type->type, data, Object::Unbox (obj));
			return obj;
		}
		case IL2CPP_TYPE_STRING:
		case IL2CPP_TYPE_CLASS:
		case IL2CPP_TYPE_OBJECT:
		case IL2CPP_TYPE_GENERICINST:
		{
			Il2CppObject* obj = NULL;
			utils::BlobReader::GetConstantValueFromBlob (type->type, data, &obj);
			return obj;
		}
		default:
			Exception::Raise (Exception::GetInvalidOperationException (StringUtils::Printf ("Attempting to get raw constant value for field of type %d", type).c_str ()));
	}

	return NULL;
}

} /* namespace Reflection */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
