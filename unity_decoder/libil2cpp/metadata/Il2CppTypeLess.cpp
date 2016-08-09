#include "il2cpp-config.h"
#include "class-internals.h"
#include "Il2CppTypeLess.h"
#include <cassert>

namespace il2cpp
{
namespace metadata
{

bool Il2CppTypeLess::operator()(const Il2CppType* t1,const Il2CppType* t2) const
{
	return Compare(t1, t2);
}

bool Il2CppTypeLess::Compare(const Il2CppType* t1,const Il2CppType* t2)
{
	if (t1->type < t2->type)
		return true;
	if (t1->byref < t2->byref)
		return true;
	if (t1->type > t2->type)
		return false;
	if (t1->byref > t2->byref)
		return false;

	switch (t1->type) {
	case IL2CPP_TYPE_VOID:
	case IL2CPP_TYPE_BOOLEAN:
	case IL2CPP_TYPE_CHAR:
	case IL2CPP_TYPE_I1:
	case IL2CPP_TYPE_U1:
	case IL2CPP_TYPE_I2:
	case IL2CPP_TYPE_U2:
	case IL2CPP_TYPE_I4:
	case IL2CPP_TYPE_U4:
	case IL2CPP_TYPE_I8:
	case IL2CPP_TYPE_U8:
	case IL2CPP_TYPE_R4:
	case IL2CPP_TYPE_R8:
	case IL2CPP_TYPE_STRING:
	case IL2CPP_TYPE_I:
	case IL2CPP_TYPE_U:
	case IL2CPP_TYPE_OBJECT:
	case IL2CPP_TYPE_TYPEDBYREF:
		return false;
	case IL2CPP_TYPE_VALUETYPE:
	case IL2CPP_TYPE_CLASS:
		return t1->data.klassIndex < t2->data.klassIndex;
	case IL2CPP_TYPE_PTR:
	case IL2CPP_TYPE_SZARRAY:
		return Compare (t1->data.type, t2->data.type);
	case IL2CPP_TYPE_ARRAY:
		if (t1->data.array->rank < t2->data.array->rank)
			return true;
		if (t1->data.array->rank > t2->data.array->rank)
			return false;
		return Compare (t1->data.array->etype, t2->data.array->etype);
	case IL2CPP_TYPE_GENERICINST: {
		const Il2CppGenericInst *i1 = t1->data.generic_class->context.class_inst;
		const Il2CppGenericInst *i2 = t2->data.generic_class->context.class_inst;
		if (i1->type_argc < i2->type_argc)
			return true;
		if (i1->type_argc > i2->type_argc)
			return false;
		bool byval_arg1_less_byval_arg2 = t1->data.generic_class->typeDefinitionIndex < t2->data.generic_class->typeDefinitionIndex;
		bool byval_arg2_less_byval_arg1 = t2->data.generic_class->typeDefinitionIndex < t1->data.generic_class->typeDefinitionIndex;
		if (byval_arg1_less_byval_arg2)
			return true;
		if (byval_arg2_less_byval_arg1)
			return false;
		/* FIXME: we should probably just compare the instance pointers directly.  */
		for (uint32_t i = 0; i < i1->type_argc; ++i) {
			bool i1_less_i2 = Compare (i1->type_argv[i], i2->type_argv[i]);
			bool i2_less_i1 = Compare (i2->type_argv[i], i1->type_argv[i]);
			bool i1_equal_i2 = !i1_less_i2 && !i2_less_i1;
			if (i1_equal_i2)
				continue;
			return i1_less_i2;
		}
		return false;
	}
	case IL2CPP_TYPE_VAR:
	case IL2CPP_TYPE_MVAR:
		if (t1->data.genericParameterIndex < t2->data.genericParameterIndex)
			return true;
		return false;
	default:
		NOT_IMPLEMENTED (Il2CppTypeLess::Compare);
		return false;
	}
	return false;
}

} /* namespace vm */
} /* namespace il2cpp */
