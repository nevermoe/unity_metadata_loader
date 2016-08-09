#pragma once

#include <stdint.h>
#include "il2cpp-config.h"
#include "object-internals.h"

struct Il2CppObject;
struct Il2CppDelegate;
struct Il2CppReflectionType;
struct Il2CppReflectionMethod;
struct Il2CppReflectionField;
struct Il2CppArray;
struct Il2CppException;
struct Il2CppReflectionModule;
struct Il2CppAssembly;
struct Il2CppAssemblyName;
struct Il2CppAppDomain;

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{
namespace Security
{
namespace Principal
{

class LIBIL2CPP_CODEGEN_API WindowsPrincipal
{
public:
	static bool IsMemberOfGroupName (Il2CppIntPtr user, Il2CppString* group);
	static bool IsMemberOfGroupId (Il2CppIntPtr user, Il2CppIntPtr group);
};

} /* namespace Principal */
} /* namespace Security */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
