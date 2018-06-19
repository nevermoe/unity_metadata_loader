#include "il2cpp-config.h"
#include "class-internals.h"
#include "Il2CppGenericContextLess.h"
#include "Il2CppGenericInstLess.h"
#include <cassert>

namespace il2cpp
{
namespace metadata
{

bool Il2CppGenericContextLess::operator() (const Il2CppGenericContext* gc1,const Il2CppGenericContext* gc2) const
{
	return Compare (gc1, gc2);
}

bool Il2CppGenericContextLess::Compare (const Il2CppGenericContext* gc1,const Il2CppGenericContext* gc2)
{
	NOT_IMPLEMENTED_NO_ASSERT (Il2CppGenericContextLess::Compare, "We should ensure GenericInst uniqueness and do direct comparison");
	// return gc1->class_inst == gc2->class_inst && gc1->method_inst == gc2->method_inst;

	if (gc1->class_inst < gc2->class_inst)
		return true;
	if (gc1->class_inst > gc2->class_inst)
		return false;

	if (gc1->class_inst)
	{
		bool class1_inst_less_class2_inst = Il2CppGenericInstLess::Compare (gc1->class_inst, gc2->class_inst);
		bool class2_inst_less_class1_inst = Il2CppGenericInstLess::Compare (gc2->class_inst, gc1->class_inst);

		if (class1_inst_less_class2_inst)
			return true;
		if (class2_inst_less_class1_inst)
			return false;
	}

	if (gc1->method_inst < gc2->method_inst)
		return true;
	if (gc1->method_inst > gc2->method_inst)
		return false;

	if (gc1->method_inst)
	{
		bool method1_inst_less_method2 = Il2CppGenericInstLess::Compare (gc1->method_inst, gc2->method_inst);
		bool method2_inst_less_method1 = Il2CppGenericInstLess::Compare (gc2->method_inst, gc1->method_inst);

		if (method1_inst_less_method2)
			return true;
		if (method2_inst_less_method1)
			return false;
	}

	return false;
}

} /* namespace vm */
} /* namespace il2cpp */
