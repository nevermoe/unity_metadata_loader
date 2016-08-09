#include "il2cpp-config.h"
#include <stddef.h>
#include <cassert>
#include <string>
#include <sstream>
#include "icalls/mscorlib/System.Reflection/MonoMethod.h"
#include <cassert>
#include "tabledefs.h"
#include "class-internals.h"
#include "metadata/Il2CppTypeVector.h"
#include "vm/Array.h"
#include "vm/Class.h"
#include "vm/Exception.h"
#include "vm/GenericContainer.h"
#include "vm/MetadataCache.h"
#include "vm/Method.h"
#include "vm/Object.h"
#include "vm/Reflection.h"
#include "vm/Runtime.h"
#include "vm/String.h"
#include "vm/Reflection.h"
#include "vm/Type.h"
#include "vm/GenericClass.h"

using namespace il2cpp::vm;
using il2cpp::metadata::Il2CppTypeVector;

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

Il2CppReflectionMethod* MonoMethod::get_base_definition(Il2CppReflectionMethod *m)
{
	const MethodInfo *method = m->method;
	Il2CppClass *klass = method->declaring_type;

	if(klass == NULL)
		return m;

	if (!(method->flags & METHOD_ATTRIBUTE_VIRTUAL) || Class::IsInterface (klass) || method->flags & METHOD_ATTRIBUTE_NEW_SLOT)
		return m;

	/*if(klass->generic_class)
		klass = klass->generic_class->container_class;*/

	for (Il2CppClass* parent = klass->parent; parent != NULL; parent = parent->parent)
	{
		if (parent->vtable_count <= method->slot)
			break;
		klass = parent;
	}

	if(klass == method->declaring_type)
		return m;

	il2cpp::vm::Class::Init(klass);

	const MethodInfo *result = klass->vtable[method->slot].method;

	if(result == NULL)
	{
		void *iterator = NULL;
		
		for(result = Class::GetMethods (klass, &iterator); result != NULL; result = Class::GetMethods (klass, &iterator))
			if(result->slot == method->slot)
				break;
	}

	if(result == NULL)
		return m;

	return il2cpp::vm::Reflection::GetMethodObject (result, klass);
}

bool MonoMethod::get_IsGenericMethod(Il2CppReflectionMethod* method)
{
	// if we are a generic method definition
	if (method->method->is_generic)
		return true;

	// is_inflated is true when a method is a generic instance or it's declaring type is a generic instance type.
	// Only return true here if we are a generic instance method
	if (method->method->is_inflated)
	{
		const Il2CppGenericContext* context = MetadataCache::GetMethodGenericContext (method->method);
		return context != NULL && context->method_inst != NULL;
	}

	return false;
}

Il2CppString *  MonoMethod::get_name(Il2CppReflectionMethod * m)
{
	const MethodInfo *method = m->method;

	IL2CPP_OBJECT_SETREF (m, name, String::New (method->name));
	return m->name;
}

mscorlib_System_Runtime_InteropServices_DllImportAttribute * MonoMethod::GetDllImportAttribute(Il2CppIntPtr)
{
	//Todo: [DllImport] is an pseudo attribute. it doesn't exist in the metadata as an attribute, but as a flag on a method.
	//however, if you use reflection to ask for attributes, it does get reported, so what needs to happen is we create an attribute
	//instanec on the fly and populate it with the data in the metadata. Turns out that if you call GetCustomAttributes() that ends
	//up calling this function. For now, we will just return an attribute, but not yet populate it with the correct data.

	Il2CppClass* typeInfo = Class::FromName(il2cpp_defaults.corlib, "System.Runtime.InteropServices", "DllImportAttribute");
	assert(typeInfo != NULL);

	return (mscorlib_System_Runtime_InteropServices_DllImportAttribute*)il2cpp::vm::Object::New(typeInfo);
}

Il2CppArray* MonoMethod::GetGenericArguments (Il2CppReflectionMethod* method)
{
	uint32_t count = 0;
	Il2CppArray* res = NULL;
	const MethodInfo* methodInfo = method->method;
	if (methodInfo->is_inflated)
	{
		const Il2CppGenericContext* context = MetadataCache::GetMethodGenericContext (methodInfo);
		if (context && context->method_inst)
		{
			const Il2CppGenericInst *inst = context->method_inst;
			count = inst->type_argc;
			res = Array::New (il2cpp_defaults.systemtype_class, count);

			for (uint32_t i = 0; i < count; i++)
				il2cpp_array_setref (res, i, il2cpp::vm::Reflection::GetTypeObject (inst->type_argv[i]));

			return res;
		}

		// method is inflated because it's owner is a generic instance type, extract method definition out of the method
		assert (methodInfo->is_generic);
		methodInfo = methodInfo->genericMethod->methodDefinition;
	}

	const Il2CppGenericContainer *container = MetadataCache::GetMethodGenericContainer (methodInfo);

	count = container->type_argc;
	res = Array::New (il2cpp_defaults.systemtype_class, count);

	for (uint32_t i = 0; i < count; i++)
	{
		const Il2CppGenericParameter *param = GenericContainer::GetGenericParameter (container, i);
		Il2CppClass *pklass = Class::FromGenericParameter (param);
		il2cpp_array_setref (res, i, il2cpp::vm::Reflection::GetTypeObject (pklass->byval_arg));
	}

	return res;
}

Il2CppObject * MonoMethod::InternalInvoke(Il2CppReflectionMethod * method, Il2CppObject * __this, Il2CppArray * params, Il2CppObject * * exc)
{
	NOT_IMPLEMENTED_ICALL_NO_ASSERT (MonoMethod::InternalInvoke, "Audit and look over commented code. Work in progress.");
/*
	 * Invoke from reflection is supposed to always be a virtual call (the API
	 * is stupid), mono_runtime_invoke_*() calls the provided method, allowing
	 * greater flexibility.
	 */
	const MethodInfo *m = method->method;
	int pcount;
	void *obj = __this;

	*exc = NULL;

	if (!(m->flags & METHOD_ATTRIBUTE_STATIC))
	{
		if (__this)
		{
			//if (!mono_class_vtable_full (mono_object_domain (method), m->klass, FALSE)) {
			//	mono_gc_wbarrier_generic_store (exc, (MonoObject*) mono_class_get_exception_for_failure (m->klass));
			//	return NULL;
			//}

			if (!Object::IsInst(__this, m->declaring_type))
			{
				assert(0);
				//mono_gc_wbarrier_generic_store (exc, (MonoObject*) mono_exception_from_name_msg (mono_defaults.corlib, "System.Reflection", "TargetException", "Object does not match target type."));
				return NULL;
			}

			m = Object::GetVirtualMethod(__this, m);
		}
		else
#if IL2CPP_ENABLE_MONO_BUG_EMULATION	// Mono doesn't throw on null 'this' if it's an instance constructor, and class libs depend on this behaviour
			if (strcmp(m->name, ".ctor"))
#endif
		{
			Exception::Raise(Exception::GetTargetException("Non-static method requires a target"));
		}
	}

	pcount = params? il2cpp::vm::Array::GetLength (params): 0;
	if (pcount != m->parameters_count) {
		assert (0);
		//mono_gc_wbarrier_generic_store (exc, (MonoObject*) mono_exception_from_name (mono_defaults.corlib, "System.Reflection", "TargetParameterCountException"));
		return NULL;
	}
	
	// TODO: Add check for abstract once types have flags
	//if ((m->declaring_type->flags & TYPE_ATTRIBUTE_ABSTRACT) && !strcmp (m->name, ".ctor") && !this) {
	//	mono_gc_wbarrier_generic_store (exc, (MonoObject*) mono_exception_from_name_msg (mono_defaults.corlib, "System.Reflection", "TargetException", "Cannot invoke constructor of an abstract class."));
	//	return NULL;
	//}
	
	if (m->declaring_type->rank && !strcmp (m->name, ".ctor")) {
		int i;
		il2cpp_array_size_t *lengths;
		il2cpp_array_size_t *lower_bounds;
		pcount = il2cpp::vm::Array::GetLength (params);
		lengths = (il2cpp_array_size_t *)alloca (sizeof (il2cpp_array_size_t) * pcount);
		for (i = 0; i < pcount; ++i)
			lengths [i] = *(il2cpp_array_size_t*) ((char*)il2cpp_array_get (params, void*, i) + sizeof (Il2CppObject));

		if (m->declaring_type->rank == pcount) {
			/* Only lengths provided. */
			lower_bounds = NULL;
		} else {
			assert (pcount == (m->declaring_type->rank * 2));
			/* lower bounds are first. */
			lower_bounds = lengths;
			lengths += m->declaring_type->rank;
		}

		return (Il2CppObject*)il2cpp::vm::Array::NewFull (m->declaring_type, lengths, lower_bounds);
	}

	// If a managed exception was thrown, we need raise it here because Runtime::Invoke catches the exception and returns a pointer to it.
	Il2CppException* exception = NULL;

	Il2CppObject *result = il2cpp::vm::Runtime::InvokeArray (m, obj, params, &exception);

	if(exception)
		Exception::Raise(exception);

	return result;
}

bool MonoMethod::get_IsGenericMethodDefinition (Il2CppReflectionMethod* method)
{
	return method->method->is_generic;
}

static std::string FormatExceptionMessageForNonConstructableGenericMethod (const MethodInfo* method, const Il2CppTypeVector& genericArguments)
{
	std::ostringstream sstream;
	sstream << "Failed to construct generic method '";
	sstream << Type::GetName (method->declaring_type->byval_arg, IL2CPP_TYPE_NAME_FORMAT_FULL_NAME);
	sstream << "::";
	sstream << Method::GetName (method);
	sstream << "' with generic arguments [";
	for (Il2CppTypeVector::const_iterator iter = genericArguments.begin (); iter != genericArguments.end (); ++iter)
	{
		if (iter != genericArguments.begin ())
			sstream << ", ";
		sstream << Type::GetName (*iter, IL2CPP_TYPE_NAME_FORMAT_FULL_NAME);
	}
	sstream << "] at runtime.";

	return sstream.str ();
}

Il2CppReflectionMethod* MonoMethod::MakeGenericMethod_impl (Il2CppReflectionMethod* method, Il2CppArray* genericArgumentTypes)
{
	const MethodInfo* genericMethodDefinition = method->method;

	if (!genericMethodDefinition->is_generic)
	{
		std::stringstream message;
		Exception::Raise (Exception::GetInvalidOperationException (message.str ().c_str ()));
	}

	uint32_t arrayLength = Array::GetLength (genericArgumentTypes);
	Il2CppTypeVector genericArguments;
	genericArguments.reserve (arrayLength);

	for (uint32_t i = 0; i < arrayLength; i++)
	{
		Il2CppReflectionType* genericArgumentType = il2cpp_array_get (genericArgumentTypes, Il2CppReflectionType*, i);
		genericArguments.push_back (genericArgumentType->type);
	}

	const MethodInfo* genericInstanceMethod = MetadataCache::GetGenericInstanceMethod (genericMethodDefinition, genericArguments);

	if (!genericInstanceMethod)
	{
		Exception::Raise (Exception::GetNotSupportedException (FormatExceptionMessageForNonConstructableGenericMethod (genericMethodDefinition, genericArguments).c_str ()));
		return NULL;
	}

	return il2cpp::vm::Reflection::GetMethodObject (genericInstanceMethod, NULL);
}

Il2CppReflectionMethod* MonoMethod::GetGenericMethodDefinition_impl (Il2CppReflectionMethod* method)
{
	if (method->method->is_generic)
		return method;

	if (!method->method->is_inflated)
		return NULL;

	const MethodInfo* methodDefinition = MetadataCache::GetGenericMethodDefinition(method->method);
	assert (methodDefinition);

	if (!methodDefinition->is_generic)
		return NULL;


	const Il2CppGenericContext* methodContext = MetadataCache::GetMethodGenericContext(method->method);
	assert (methodContext);

	if (methodContext->class_inst)
	{
		NOT_IMPLEMENTED_ICALL (MonoMethod::GetGenericMethodDefinition_impl);
	}

	return il2cpp::vm::Reflection::GetMethodObject (const_cast<MethodInfo*> (methodDefinition), NULL);
}

} /* namespace Reflection */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
