#pragma once

#include <stdint.h>
#include "il2cpp-config.h"
#include "object-internals.h"

struct mscorlib_System_Runtime_InteropServices_DllImportAttribute;
struct mscorlib_System_Reflection_MethodInfo;

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

class LIBIL2CPP_CODEGEN_API MonoMethod
{
public:
	static mscorlib_System_Runtime_InteropServices_DllImportAttribute * GetDllImportAttribute (Il2CppIntPtr);
	static Il2CppArray * GetGenericArguments (Il2CppReflectionMethod *);
	static Il2CppReflectionMethod* GetGenericMethodDefinition_impl (Il2CppReflectionMethod* method);
	static Il2CppObject * InternalInvoke (Il2CppReflectionMethod * method, Il2CppObject * __this, Il2CppArray * params, Il2CppObject * * exc);
	static bool get_IsGenericMethod (Il2CppReflectionMethod *);
	static bool get_IsGenericMethodDefinition (Il2CppReflectionMethod *);
	static Il2CppReflectionMethod* get_base_definition (Il2CppReflectionMethod *);
	static Il2CppString *  get_name (Il2CppReflectionMethod * m);
	static Il2CppReflectionMethod* MakeGenericMethod_impl (Il2CppReflectionMethod *, Il2CppArray *);

};
	
} /* namespace Reflection */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
