#include "il2cpp-config.h"
#include "class-internals.h"
#include "Il2CppGenericClassCompare.h"
#include "Il2CppGenericInstCompare.h"
#include <cassert>

namespace il2cpp
{
namespace metadata
{

bool Il2CppGenericClassCompare::operator() (const Il2CppGenericClass* gc1,const Il2CppGenericClass* gc2) const
{
	return Compare (gc1, gc2);
}

bool Il2CppGenericClassCompare::Compare (const Il2CppGenericClass* gc1,const Il2CppGenericClass* gc2)
{
	const Il2CppGenericInst *i1 = gc1->context.class_inst;
	const Il2CppGenericInst *i2 = gc2->context.class_inst;

	if (gc1->typeDefinitionIndex != gc2->typeDefinitionIndex)
		return false;

	return Il2CppGenericInstCompare::Compare (i1, i2);
}

} /* namespace vm */
} /* namespace il2cpp */
