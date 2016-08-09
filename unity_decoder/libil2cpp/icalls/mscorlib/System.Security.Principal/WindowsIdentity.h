#pragma once

#include "il2cpp-config.h"
#include "object-internals.h"

struct Il2CppString;

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

class LIBIL2CPP_CODEGEN_API WindowsIdentity
{
public:
	static Il2CppIntPtr GetUserToken (Il2CppString* username);
	static Il2CppArray* _GetRoles (Il2CppIntPtr token);
	static Il2CppString* GetTokenName (Il2CppIntPtr token);
	static Il2CppIntPtr GetCurrentToken ();
};

} /* namespace Principal */
} /* namespace Security */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
