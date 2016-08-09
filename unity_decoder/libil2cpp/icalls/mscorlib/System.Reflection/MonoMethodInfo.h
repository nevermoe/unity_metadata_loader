#pragma once

#include <stdint.h>
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

class LIBIL2CPP_CODEGEN_API MonoMethodInfo
{
public:
	static void get_method_info (Il2CppIntPtr methodPtr, Il2CppMethodInfo* info);
	static void* /* System.Reflection.Emit.UnmanagedMarshal */ get_retval_marshal (Il2CppIntPtr handle);
	static Il2CppArray* get_parameter_info (Il2CppIntPtr methodPtr, Il2CppReflectionMethod* member);
};
	
} /* namespace Reflection */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
