#pragma once

#include "il2cpp-config.h"
#include "object-internals.h"

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

class LIBIL2CPP_CODEGEN_API MethodBase
{
public:
	static Il2CppReflectionMethod* GetCurrentMethod();
	static void* /* System.Reflection.MethodBody */ GetMethodBodyInternal (Il2CppIntPtr handle);
	static Il2CppReflectionMethod* GetMethodFromHandleInternalType (Il2CppIntPtr method,Il2CppIntPtr type);
};
	
} /* namespace Reflection */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
