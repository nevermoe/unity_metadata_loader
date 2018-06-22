#include "il2cpp-config.h"
#include "class-internals.h"
#include "Il2CppGenericInstLess.h"
#include "Il2CppTypeLess.h"
#include <cassert>

namespace il2cpp
{
namespace metadata
{

bool Il2CppGenericInstLess::operator() (const Il2CppGenericInst* t1,const Il2CppGenericInst* t2) const
{
	return Compare(t1, t2);
}

bool Il2CppGenericInstLess::Compare (const Il2CppGenericInst* t1,const Il2CppGenericInst* t2)
{
	if (t1->type_argc < t2->type_argc)
		return true;
	if (t1->type_argc > t2->type_argc)
		return false;

	for (size_t i = 0; i < t1->type_argc; ++i)
	{
		bool t1_less_t2 = Il2CppTypeLess::Compare (t1->type_argv[i], t2->type_argv[i]);
		bool t2_less_t1 = Il2CppTypeLess::Compare (t2->type_argv[i], t1->type_argv[i]);
		bool t1_equal_t2 = !t1_less_t2 && !t2_less_t1;

		if (t1_equal_t2)
			continue;
		return t1_less_t2;
	}

	return false;
}

} /* namespace vm */
} /* namespace il2cpp */
