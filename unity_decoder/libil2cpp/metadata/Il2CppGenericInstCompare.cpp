#include "il2cpp-config.h"
#include "class-internals.h"
#include "Il2CppGenericInstCompare.h"
#include "Il2CppTypeCompare.h"
#include <cassert>

namespace il2cpp
{
namespace metadata
{

bool Il2CppGenericInstCompare::operator() (const Il2CppGenericInst* t1,const Il2CppGenericInst* t2) const 
{
	return Compare(t1, t2);
}

bool Il2CppGenericInstCompare::Compare (const Il2CppGenericInst* t1,const Il2CppGenericInst* t2)
{
	if (t1->type_argc != t2->type_argc)
		return false;

	for (size_t i = 0; i < t1->type_argc; ++i)
	{
		if (!Il2CppTypeCompare::Compare (t1->type_argv[i], t2->type_argv[i]))
			return false;
	}

	return true;
}

} /* namespace vm */
} /* namespace il2cpp */
