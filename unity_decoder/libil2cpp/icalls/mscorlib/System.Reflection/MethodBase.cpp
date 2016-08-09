#include "il2cpp-config.h"
#include <stddef.h>
#include "icalls/mscorlib/System.Reflection/MethodBase.h"
#include <cassert>
#include "class-internals.h"
#include "vm/Class.h"
#include "vm/Exception.h"
#include "vm/GenericClass.h"
#include "vm/Reflection.h"
#include "vm/StackTrace.h"

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


static Il2CppClass* il2cpp_class_get_generic_type_definition(Il2CppClass *klass)
{
	return klass->generic_class ? il2cpp::vm::GenericClass::GetTypeDefinition (klass->generic_class) : klass;
}

Il2CppReflectionMethod * MethodBase::GetMethodFromHandleInternalType(Il2CppIntPtr method,Il2CppIntPtr type)
{
	Il2CppClass *klass = NULL;
	MethodInfo* methodInfo = (MethodInfo*)method.m_value;
	if (type.m_value)
	{
		klass = vm::Class::FromIl2CppType((Il2CppType*)type.m_value);
		if (il2cpp_class_get_generic_type_definition(methodInfo->declaring_type) != il2cpp_class_get_generic_type_definition(klass))
			return NULL;

		// See the VerifyTwoArgumentGetMethodFromHandleWithGenericType for the failing test. Once we have support for inflating methods
		// we can implement this case as well and make that test pass.
		if (methodInfo->declaring_type != klass)
			NOT_IMPLEMENTED_ICALL(MethodBase::GetMethodFromHandleInternalType);
	}
	else
	{
		klass = methodInfo->declaring_type;
	}
	return il2cpp::vm::Reflection::GetMethodObject(methodInfo, klass);
}

Il2CppReflectionMethod* MethodBase::GetCurrentMethod ()
{
	NOT_SUPPORTED_IL2CPP(MethodBase::GetCurrentMethod, "This icall is not supported by il2cpp. Use the il2cpp_codegen_get_method_object intrinsic instead.");

	return NULL;
}

void* /* System.Reflection.MethodBody */ MethodBase::GetMethodBodyInternal (Il2CppIntPtr handle)
{
	NOT_SUPPORTED_IL2CPP (MethodBase::GetMethodBodyInternal, "This icall is not supported by il2cpp.");
	
	return 0;
}

} /* namespace Reflection */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
