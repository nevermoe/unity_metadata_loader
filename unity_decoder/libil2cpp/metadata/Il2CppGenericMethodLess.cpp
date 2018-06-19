#include "il2cpp-config.h"
#include "class-internals.h"
#include "Il2CppGenericMethodLess.h"
#include "Il2CppGenericContextLess.h"
#include <cassert>

namespace il2cpp
{
namespace metadata
{


bool Il2CppGenericMethodLess::operator() (const Il2CppGenericMethod* m1, const Il2CppGenericMethod* m2) const
{
	return Compare(m1, m2);
}

bool Il2CppGenericMethodLess::Compare (const Il2CppGenericMethod* m1, const Il2CppGenericMethod* m2)
{
	if (m1->methodDefinition < m2->methodDefinition)
		return true;
	if (m1->methodDefinition > m2->methodDefinition)
		return false;

	return Il2CppGenericContextLess::Compare (&m1->context, &m2->context);
}

} /* namespace vm */
} /* namespace il2cpp */
