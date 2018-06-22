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

class LIBIL2CPP_CODEGEN_API WindowsImpersonationContext
{
public:
	static bool RevertToSelf ();
	static Il2CppIntPtr DuplicateToken (Il2CppIntPtr token);
	static bool SetCurrentToken (Il2CppIntPtr token);
	static bool CloseToken (Il2CppIntPtr token);
};

} /* namespace Principal */
} /* namespace Security */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
