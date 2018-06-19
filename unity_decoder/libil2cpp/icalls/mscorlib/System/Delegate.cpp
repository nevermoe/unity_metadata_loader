#include "il2cpp-config.h"
#include <cassert>
#include "icalls/mscorlib/System/Delegate.h"
#include "vm/Class.h"
#include "vm/Method.h"
#include "vm/Object.h"
#include "vm/Type.h"
#include "class-internals.h"
#include "object-internals.h"

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{

Il2CppDelegate * Delegate::CreateDelegate_internal (Il2CppReflectionType *__type, Il2CppObject *target, Il2CppReflectionMethod *info, bool throwOnBindFailure)
{
	Il2CppClass *delegate_class = il2cpp::vm::Class::FromIl2CppType(__type->type);
	const MethodInfo *method = info->method;

	assert(delegate_class->parent == il2cpp_defaults.multicastdelegate_class);

	//if (mono_security_get_mode () == MONO_SECURITY_MODE_CORE_CLR) {
	//	if (!mono_security_core_clr_ensure_delegate_creation (method, throwOnBindFailure))
	//		return NULL;
	//}

	Il2CppObject* delegate = il2cpp::vm::Object::New(delegate_class);
	Il2CppMethodPointer func = method->methodPointer;

	il2cpp::vm::Type::ConstructDelegate((Il2CppDelegate*)delegate, target, func, method);

	return (Il2CppDelegate*)delegate;
}

void Delegate::SetMulticastInvoke (Il2CppDelegate * delegate)
{
	const MethodInfo* invokeMethod = il2cpp::vm::Class::GetMethodFromName (delegate->object.klass, "Invoke", -1);
	delegate->invoke_impl = invokeMethod->invoker_method;
}

} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
