#pragma once

#include "il2cpp-config.h"

#include <cassert>
#include <cstdlib>
#include <limits>
#include <string>
#include <math.h>
#include <vector>

#include "il2cpp-api.h"
#include "object-internals.h"
#include "class-internals.h"
#include "tabledefs.h"
#include "icallincludes.h"

#include "utils/RegisterRuntimeInitializeAndCleanup.h"
#include "utils/StringUtils.h"
#include "utils/StringView.h"

#include "metadata/GenericMethod.h"
#include "vm/Array.h"
#include "vm/Assembly.h"
#include "vm/Atomic.h"
#include "vm/CCW.h"
#include "vm/Class.h"
#include "vm/ComUtils.h"
#include "vm/Domain.h"
#include "vm/Exception.h"
#include "vm/InternalCalls.h"
#include "vm/LastError.h"
#include "vm/MarshalAlloc.h"
#include "vm/MetadataCache.h"
#include "vm/Method.h"
#include "vm/Object.h"
#include "vm/PlatformInvoke.h"
#include "vm/Profiler.h"
#include "vm/RCW.h"
#include "vm/Reflection.h"
#include "vm/Runtime.h"
#include "vm/StackTrace.h"
#include "vm/String.h"
#include "vm/Thread.h"
#include "vm/Type.h"
#include "vm/WindowsRuntime.h"

#ifdef _MSC_VER
#define IL2CPP_DEBUG_BREAK() __debugbreak()
#define IL2CPP_DISABLE_OPTIMIZATIONS __pragma(optimize("", off))
#define IL2CPP_ENABLE_OPTIMIZATIONS __pragma(optimize("", on))
#else
#define IL2CPP_DEBUG_BREAK()
#define IL2CPP_DISABLE_OPTIMIZATIONS
#define IL2CPP_ENABLE_OPTIMIZATIONS
#endif

struct ProfilerMethodSentry
{
	ProfilerMethodSentry(const MethodInfo* method) 
#if IL2CPP_ENABLE_PROFILER
		: m_method(method)
#endif
	{
#if IL2CPP_ENABLE_PROFILER
		il2cpp::vm::Profiler::MethodEnter (m_method);
#endif
	}

	~ProfilerMethodSentry()
	{
#if IL2CPP_ENABLE_PROFILER
		il2cpp::vm::Profiler::MethodExit (m_method);
#endif
	}

private:
	const MethodInfo* m_method;
};

struct StackTraceSentry
{
	StackTraceSentry(const MethodInfo* method) : m_method(method)
	{
		Il2CppStackFrameInfo frame_info;

		frame_info.method = method;

#if IL2CPP_DEBUGGER_ENABLED
		frame_info.id = -1;
		frame_info.il_offset = 0;
		frame_info.type = FRAME_TYPE_MANAGED;
#endif
		il2cpp::vm::StackTrace::PushFrame (frame_info);
	}

#if IL2CPP_DEBUGGER_ENABLED
	StackTraceSentry(const MethodInfo* method, void* this_ptr, void **params, int32_t params_count, void **locals, int32_t locals_count) : m_method(method)
	{
		Il2CppStackFrameInfo frame_info;

		frame_info.id = -1;
		frame_info.this_ptr = this_ptr;
		frame_info.method = method;
		frame_info.il_offset = 0;
		frame_info.type = FRAME_TYPE_MANAGED;
		frame_info.params = params;
		frame_info.params_count = params_count;
		frame_info.locals = locals;
		frame_info.locals_count = locals_count;
		il2cpp::vm::StackTrace::PushFrame (frame_info);
	}
#endif

	~StackTraceSentry()
	{
		il2cpp::vm::StackTrace::PopFrame ();
	}

private:
	const MethodInfo* m_method;
};

// TODO: This file should contain all the functions and type declarations needed for the generated code.
// Hopefully, we stop including everything in the generated code and know exactly what dependencies we have.
// Note that all parameter and return types should match the generated types not the runtime types.

inline void il2cpp_codegen_register (const Il2CppCodeRegistration* const codeRegistration, const Il2CppMetadataRegistration* const metadataRegistration, const Il2CppCodeGenOptions* const codeGenOptions)
{
	il2cpp::vm::MetadataCache::Register (codeRegistration, metadataRegistration, codeGenOptions);
}

#include "GeneratedCodeGen.h"

// type registration

inline void* il2cpp_codegen_get_thread_static_data (Il2CppClass* klass)
{
	return il2cpp::vm::Thread::GetThreadStaticData (klass->thread_static_fields_offset);
}

inline Il2CppCodeGenString* il2cpp_codegen_string_new_wrapper (const char* str)
{
	return (Il2CppCodeGenString*)il2cpp::vm::String::NewWrapper (str);
}

inline Il2CppCodeGenType* il2cpp_codegen_type_get_object (const Il2CppType* type)
{
	return (Il2CppCodeGenType*)il2cpp::vm::Reflection::GetTypeObject (type);
}

inline NORETURN void il2cpp_codegen_raise_exception (Il2CppCodeGenException *ex)
{
	il2cpp::vm::Exception::Raise ((Il2CppException*)ex);
}

// This function exists to help with generation of callstacks for exceptions
// on iOS. There we call the backtrace function, which does not play nicely with
// NORETURN, since the compiler eliminates the method prologue code setting up
// the address of the return frame (which makes sense). So on iOS we need to make
// the NORETURN define do nothing, then we use this dummy method which has the
// attribute for clang on iOS defined to prevent clang compiler errors for
// method that end by throwing a managed exception.
REAL_NORETURN IL2CPP_NO_INLINE static void il2cpp_codegen_no_return()
{
	IL2CPP_UNREACHABLE;
}

inline void il2cpp_codegen_raise_execution_engine_exception_if_method_is_not_found(const MethodInfo* method)
{
	il2cpp::vm::Runtime::RaiseExecutionEngineExceptionIfMethodIsNotFound(method);
}

inline Il2CppCodeGenException* il2cpp_codegen_get_argument_exception(const char* param, const char* msg)
{
	return (Il2CppCodeGenException*)il2cpp::vm::Exception::GetArgumentException(param, msg);
}

inline Il2CppCodeGenException* il2cpp_codegen_get_argument_null_exception(const char* param)
{
	return (Il2CppCodeGenException*)il2cpp::vm::Exception::GetArgumentNullException(param);
}

inline Il2CppCodeGenException* il2cpp_codegen_get_overflow_exception()
{
	return (Il2CppCodeGenException*)il2cpp::vm::Exception::GetOverflowException("Arithmetic operation resulted in an overflow.");
}

inline Il2CppCodeGenException* il2cpp_codegen_get_not_supported_exception(const char* msg)
{
	return (Il2CppCodeGenException*)il2cpp::vm::Exception::GetNotSupportedException(msg);
}

inline Il2CppCodeGenException* il2cpp_codegen_get_array_type_mismatch_exception()
{
	return (Il2CppCodeGenException*)il2cpp::vm::Exception::GetArrayTypeMismatchException();
}

inline Il2CppCodeGenException* il2cpp_codegen_get_invalid_operation_exception(const char* msg)
{
	return (Il2CppCodeGenException*)il2cpp::vm::Exception::GetInvalidOperationException(msg);
}

inline Il2CppCodeGenException* il2cpp_codegen_get_marshal_directive_exception(const char* msg)
{
	return (Il2CppCodeGenException*)il2cpp::vm::Exception::GetMarshalDirectiveException(msg);
}

inline Il2CppCodeGenException* il2cpp_codegen_get_missing_method_exception(const char* msg)
{
	return (Il2CppCodeGenException*)il2cpp::vm::Exception::GetMissingMethodException(msg);
}

inline Il2CppCodeGenException* il2cpp_codegen_get_maximum_nested_generics_exception()
{
	return (Il2CppCodeGenException*)il2cpp::vm::Exception::GetMaxmimumNestedGenericsException();
}

// OpCode.IsInst

inline Il2CppCodeGenObject* IsInst(Il2CppCodeGenObject *obj, Il2CppClass* targetType)
{
	return (Il2CppCodeGenObject*)il2cpp::vm::Object::IsInst((Il2CppObject*)obj, targetType);
}

inline Il2CppCodeGenObject* IsInstSealed(Il2CppCodeGenObject *obj, Il2CppClass* targetType)
{
#if IL2CPP_DEBUG
	assert ((targetType->flags & TYPE_ATTRIBUTE_SEALED) != 0);
	assert ((targetType->flags & TYPE_ATTRIBUTE_INTERFACE) == 0);
#endif
	if (!obj)
		return NULL;

	// optimized version to compare sealed classes
	return (((Il2CppObject*)obj)->klass == targetType ? obj: NULL);
}

inline Il2CppCodeGenObject* IsInstClass(Il2CppCodeGenObject *obj, Il2CppClass* targetType)
{
#if IL2CPP_DEBUG
	assert ((targetType->flags & TYPE_ATTRIBUTE_INTERFACE) == 0);
#endif
	if (!obj)
		return NULL;

	// optimized version to compare classes
	return il2cpp::vm::Class::HasParentUnsafe (((Il2CppObject*)obj)->klass, targetType) ? obj: NULL;
}

// OpCode.Castclass

inline Il2CppCodeGenObject* Castclass(Il2CppCodeGenObject *obj, Il2CppClass* targetType)
{
	if (!obj)
		return NULL;

	Il2CppObject* result = il2cpp::vm::Object::IsInst((Il2CppObject*)obj, targetType);
	if (result)
		return (Il2CppCodeGenObject*)result;
	
	il2cpp::vm::Exception::Raise ((Il2CppException*)il2cpp::vm::Exception::GetInvalidCastException (il2cpp::vm::Exception::FormatInvalidCastException(((Il2CppObject*)obj)->klass->element_class, targetType).c_str()));
	return NULL;
}

inline Il2CppCodeGenObject* CastclassSealed(Il2CppCodeGenObject *obj, Il2CppClass* targetType)
{
	if (!obj)
		return NULL;

	Il2CppCodeGenObject* result = IsInstSealed(obj, targetType);
	if (result)
		return result;
	
	il2cpp::vm::Exception::Raise ((Il2CppException*)il2cpp::vm::Exception::GetInvalidCastException (il2cpp::vm::Exception::FormatInvalidCastException(((Il2CppObject*)obj)->klass->element_class, targetType).c_str()));
	return NULL;
}

inline Il2CppCodeGenObject* CastclassClass(Il2CppCodeGenObject *obj, Il2CppClass* targetType)
{
	if (!obj)
		return NULL;

	Il2CppCodeGenObject* result = IsInstClass(obj, targetType);
	if (result)
		return result;
	
	il2cpp::vm::Exception::Raise ((Il2CppException*)il2cpp::vm::Exception::GetInvalidCastException (il2cpp::vm::Exception::FormatInvalidCastException(((Il2CppObject*)obj)->klass->element_class, targetType).c_str()));
	return NULL;
}

// OpCode.Box

inline Il2CppCodeGenObject* Box(Il2CppClass* type, void* data)
{
	return (Il2CppCodeGenObject*)il2cpp::vm::Object::Box(type, data);
}

// OpCode.UnBox

inline void* UnBox(Il2CppCodeGenObject* obj)
{
	if (!obj){
		il2cpp::vm::Exception::RaiseNullReferenceException ();
		il2cpp_codegen_no_return();
	}

	return il2cpp::vm::Object::Unbox((Il2CppObject*)obj);
}

inline void* UnBox(Il2CppCodeGenObject* obj, Il2CppClass* klass)
{
	if (!obj){
		il2cpp::vm::Exception::RaiseNullReferenceException ();
		il2cpp_codegen_no_return();
	}

	Il2CppObject* obj2 = (Il2CppObject*)obj;
	if (obj2->klass->element_class != klass->element_class)
		il2cpp::vm::Exception::Raise ((Il2CppException*)il2cpp::vm::Exception::GetInvalidCastException (il2cpp::vm::Exception::FormatInvalidCastException(obj2->klass->element_class, klass->element_class).c_str()));

	return il2cpp::vm::Object::Unbox(obj2);
}

inline void UnBoxNullable(Il2CppCodeGenObject* obj, Il2CppClass* klass, void* storage)
{
	return il2cpp::vm::Object::UnboxNullable(reinterpret_cast<Il2CppObject*>(obj), klass, storage);
}

inline void* il2cpp_codegen_get_delegate_this(Il2CppCodeGenMulticastDelegate* codegenDelegate)
{
	Il2CppDelegate* delegate = (Il2CppDelegate*)codegenDelegate;
	return delegate->target;
}

inline uint32_t il2cpp_codegen_sizeof(Il2CppClass* klass)
{
	if (!klass->valuetype)
	{
		return sizeof(void*);
	}

	return il2cpp::vm::Class::GetInstanceSize(klass) - sizeof(Il2CppObject);
}

#include "GeneratedVirtualInvokers.h"
#include "GeneratedInterfaceInvokers.h"
#include "GeneratedGenericVirtualInvokers.h"
#include "GeneratedGenericInterfaceInvokers.h"

// OpCode.Ldtoken

inline Il2CppCodeGenRuntimeTypeHandle LoadTypeToken(const Il2CppType* ptr)
{
	Il2CppCodeGenIntPtr intptr;
	intptr.set_m_value_0(const_cast<Il2CppType*>(ptr));

	Il2CppCodeGenRuntimeTypeHandle handle;
	handle.set_value_0(intptr);

	return handle;
}

inline Il2CppCodeGenRuntimeFieldHandle LoadFieldToken(void* ptr)
{
	Il2CppCodeGenIntPtr intptr;
	intptr.set_m_value_0(ptr);

	Il2CppCodeGenRuntimeFieldHandle handle;
	handle.set_value_0(intptr);

	return handle;
}

inline Il2CppCodeGenRuntimeArgumentHandle LoadArgList()
{
	Il2CppCodeGenIntPtr intptr;
	intptr.set_m_value_0(NULL);

	Il2CppCodeGenRuntimeArgumentHandle handle;
	handle.set_args_0(intptr);

	assert(false && "__arglist usage not supported.");
	return handle;
}

inline Il2CppCodeGenRuntimeMethodHandle LoadMethodToken(const MethodInfo* ptr)
{
	Il2CppCodeGenIntPtr intptr;
	intptr.set_m_value_0(const_cast<MethodInfo*>(ptr));

	Il2CppCodeGenRuntimeMethodHandle handle;
	handle.set_value_0(intptr);

	return handle;
}

inline Il2CppClass* InitializedTypeInfo (Il2CppClass* Il2CppClass)
{
	il2cpp::vm::Class::Init (Il2CppClass);
	return Il2CppClass;
}

inline Il2CppClass* il2cpp_codegen_class_from_type (const Il2CppType *type)
{
	return InitializedTypeInfo (il2cpp::vm::Class::FromIl2CppType (type));
}

template<typename T>
inline T InterlockedCompareExchangeImpl (T* location, T value, T comparand)
{
	return (T)il2cpp::icalls::mscorlib::System::Threading::Interlocked::CompareExchange_T((void**)location, value, comparand);
}

template<typename T>
inline T InterlockedExchangeImpl (T* location, T value)
{
	return (T)il2cpp::icalls::mscorlib::System::Threading::Interlocked::ExchangePointer((void**)location, value);
}

inline void ArrayGetGenericValueImpl (Il2CppCodeGenArray* __this, int32_t pos, void* value)
{
	memcpy(value, ((uint8_t*)__this) + sizeof(Il2CppCodeGenArray) + pos*__this->klass->element_size, __this->klass->element_size);
}

inline void ArraySetGenericValueImpl (Il2CppCodeGenArray * __this, int32_t pos, void* value)
{
	memcpy(((uint8_t*)__this) + sizeof(Il2CppCodeGenArray) + pos*__this->klass->element_size, value, __this->klass->element_size);
}

inline Il2CppCodeGenArray* SZArrayNew (Il2CppClass* arrayType, uint32_t length)
{
	il2cpp::vm::Class::Init (arrayType);
	return (Il2CppCodeGenArray*)il2cpp::vm::Array::NewSpecific (arrayType, length);
}

inline Il2CppCodeGenArray* GenArrayNew(Il2CppClass* arrayType, il2cpp_array_size_t* dimensions)
{
	return (Il2CppCodeGenArray*)il2cpp::vm::Array::NewFull(arrayType, dimensions, NULL);
}

// Performance optimization as detailed here: http://blogs.msdn.com/b/clrcodegeneration/archive/2009/08/13/array-bounds-check-elimination-in-the-clr.aspx
// Since array size is a signed int32_t, a single unsigned check can be performed to determine if index is less than array size.
// Negative indices will map to a unsigned number greater than or equal to 2^31 which is larger than allowed for a valid array.
#define IL2CPP_ARRAY_BOUNDS_CHECK(a,index) \
	do { \
		if (((uint32_t)(index)) >= ((uint32_t)(a)->max_length)) il2cpp::vm::Exception::Raise (il2cpp::vm::Exception::GetIndexOutOfRangeException()); \
	} while (0)

inline int32_t il2cpp_codegen_class_interface_offset (Il2CppClass *klass, Il2CppClass *itf)
{
	int32_t offset = il2cpp::vm::Class::GetInterfaceOffset (klass, itf);
	return offset != -1 ? offset : 0;
}

inline bool il2cpp_codegen_class_is_assignable_from (Il2CppClass *klass, Il2CppClass *oklass)
{
	return il2cpp::vm::Class::IsAssignableFrom (klass, oklass);
}

inline Il2CppObject* il2cpp_codegen_object_new (Il2CppClass *klass)
{
	return il2cpp::vm::Object::New (klass);
}

inline Il2CppMethodPointer il2cpp_codegen_resolve_icall (const char* name)
{
	Il2CppMethodPointer method = il2cpp::vm::InternalCalls::Resolve (name);
	if (!method)
	{
		il2cpp::vm::Exception::Raise(il2cpp::vm::Exception::GetMissingMethodException(name));
	}
	return method;
}

template <typename FunctionPointerType, size_t dynamicLibraryLength, size_t entryPointLength>
inline FunctionPointerType il2cpp_codegen_resolve_pinvoke(const Il2CppNativeChar (&nativeDynamicLibrary)[dynamicLibraryLength], const char (&entryPoint)[entryPointLength], 
	Il2CppCallConvention callingConvention, Il2CppCharSet charSet, int parameterSize, bool isNoMangle)
{
	const PInvokeArguments pinvokeArgs =
	{
		il2cpp::utils::StringView<Il2CppNativeChar>(nativeDynamicLibrary),
		il2cpp::utils::StringView<char>(entryPoint),
		callingConvention,
		charSet,
		parameterSize,
		isNoMangle
	};

	return reinterpret_cast<FunctionPointerType>(il2cpp::vm::PlatformInvoke::Resolve(pinvokeArgs));
}

template <typename T>
inline T* il2cpp_codegen_marshal_allocate()
{
	return static_cast<T*>(il2cpp::vm::MarshalAlloc::Allocate(sizeof(T)));
}

template <typename T>
inline T* il2cpp_codegen_marshal_allocate_array(size_t length)
{
	return static_cast<T*>(il2cpp::vm::MarshalAlloc::Allocate((il2cpp_array_size_t)(sizeof(T) * length)));
}

inline char* il2cpp_codegen_marshal_string(Il2CppCodeGenString* il2CppString)
{
	return il2cpp::vm::PlatformInvoke::MarshalCSharpStringToCppString((Il2CppString*)il2CppString);
}

inline void il2cpp_codegen_marshal_string_fixed(Il2CppCodeGenString* il2CppString, char* buffer, int numberOfCharacters)
{
	return il2cpp::vm::PlatformInvoke::MarshalCSharpStringToCppStringFixed((Il2CppString*)il2CppString, buffer, numberOfCharacters);
}

inline Il2CppChar* il2cpp_codegen_marshal_wstring(Il2CppCodeGenString* il2CppString)
{
	return il2cpp::vm::PlatformInvoke::MarshalCSharpStringToCppWString((Il2CppString*)il2CppString);
}

inline void il2cpp_codegen_marshal_wstring_fixed(Il2CppCodeGenString* il2CppString, Il2CppChar* buffer, int numberOfCharacters)
{
	return il2cpp::vm::PlatformInvoke::MarshalCSharpStringToCppWStringFixed((Il2CppString*)il2CppString, buffer, numberOfCharacters);
}

inline Il2CppChar* il2cpp_codegen_marshal_bstring(Il2CppCodeGenString* il2CppString)
{
	return il2cpp::vm::PlatformInvoke::MarshalCSharpStringToCppBString((Il2CppString*)il2CppString);
}

inline Il2CppCodeGenString* il2cpp_codegen_marshal_string_result(const char* value)
{
	return (Il2CppCodeGenString*)il2cpp::vm::PlatformInvoke::MarshalCppStringToCSharpStringResult(value);
}

inline Il2CppCodeGenString* il2cpp_codegen_marshal_wstring_result(const Il2CppChar* value)
{
	return (Il2CppCodeGenString*)il2cpp::vm::PlatformInvoke::MarshalCppWStringToCSharpStringResult(value);
}

inline Il2CppCodeGenString* il2cpp_codegen_marshal_bstring_result(const Il2CppChar* value)
{
	return (Il2CppCodeGenString*)il2cpp::vm::PlatformInvoke::MarshalCppBStringToCSharpStringResult(value);
}

inline void il2cpp_codegen_marshal_free_bstring(Il2CppChar* value)
{
	il2cpp::vm::PlatformInvoke::MarshalFreeBString(value);
}

inline char* il2cpp_codegen_marshal_string_builder(Il2CppCodeGenStringBuilder* stringBuilder)
{
	return il2cpp::vm::PlatformInvoke::MarshalStringBuilder((Il2CppStringBuilder*)stringBuilder);
}

inline Il2CppChar* il2cpp_codegen_marshal_wstring_builder(Il2CppCodeGenStringBuilder* stringBuilder)
{
	return il2cpp::vm::PlatformInvoke::MarshalWStringBuilder((Il2CppStringBuilder*)stringBuilder);
}

inline void il2cpp_codegen_marshal_string_builder_result(Il2CppCodeGenStringBuilder* stringBuilder, char* buffer)
{
	il2cpp::vm::PlatformInvoke::MarshalStringBuilderResult((Il2CppStringBuilder*)stringBuilder, buffer);
}

inline void il2cpp_codegen_marshal_wstring_builder_result(Il2CppCodeGenStringBuilder* stringBuilder, Il2CppChar* buffer)
{
	il2cpp::vm::PlatformInvoke::MarshalWStringBuilderResult((Il2CppStringBuilder*)stringBuilder, buffer);
}

template <typename ElementType>
inline ElementType* il2cpp_codegen_marshal_array(Il2CppCodeGenArray* a)
{
	return il2cpp::vm::PlatformInvoke::MarshalArray<ElementType>((Il2CppArray*)a);
}

template <typename ElementType>
inline Il2CppArray* il2cpp_codegen_marshal_array_result(Il2CppClass* type, ElementType* cppArray, size_t size)
{
	return il2cpp::vm::PlatformInvoke::MarshalArrayResult(type, cppArray, size);
}

template <typename ElementType>
inline void il2cpp_codegen_marshal_array_out(ElementType* cppArray, Il2CppCodeGenArray* managedArray)
{
	il2cpp::vm::PlatformInvoke::MarshalArrayOut(cppArray, (Il2CppArray*)managedArray);
}

inline char** il2cpp_codegen_marshal_allocate_native_string_array(size_t size)
{
	return il2cpp::vm::PlatformInvoke::MarshalAllocateNativeStringArray(size);
}

inline Il2CppChar** il2cpp_codegen_marshal_allocate_native_wstring_array(size_t size)
{
	return il2cpp::vm::PlatformInvoke::MarshalAllocateNativeWStringArray(size);
}

inline Il2CppChar** il2cpp_codegen_marshal_allocate_native_bstring_array(size_t size)
{
	return il2cpp::vm::PlatformInvoke::MarshalAllocateNativeBStringArray(size);
}

inline char** il2cpp_codegen_marshal_string_array(Il2CppCodeGenArray* a)
{
	if (a == NULL)
		return NULL;

	// Mono adds a null terminator on the a string array, so we will do the same.
	char** nativeArray = il2cpp_codegen_marshal_allocate_array<char*>(a->max_length + 1);
	il2cpp::vm::PlatformInvoke::MarshalStringArray((Il2CppArray*)a, nativeArray);
	return nativeArray;
}

inline Il2CppChar** il2cpp_codegen_marshal_wstring_array(Il2CppCodeGenArray* a)
{
	if (a == NULL)
		return NULL;

	// Mono adds a null terminator on the a string array, so we will do the same.
	Il2CppChar** nativeArray = il2cpp_codegen_marshal_allocate_array<Il2CppChar*>(a->max_length + 1);
	il2cpp::vm::PlatformInvoke::MarshalWStringArray((Il2CppArray*)a, nativeArray);
	return nativeArray;
}

inline Il2CppChar** il2cpp_codegen_marshal_bstring_array(Il2CppCodeGenArray* a)
{
	if (a == NULL)
		return NULL;

	// Mono adds a null terminator on the a string array, so we will do the same.
	Il2CppChar** nativeArray = il2cpp_codegen_marshal_allocate_array<Il2CppChar*>(a->max_length + 1);
	il2cpp::vm::PlatformInvoke::MarshalBStringArray((Il2CppArray*)a, nativeArray);
	return nativeArray;
}

inline Il2CppCodeGenArray* il2cpp_codegen_marshal_string_array_result(char** a, size_t size)
{
	return (Il2CppCodeGenArray*)il2cpp::vm::PlatformInvoke::MarshalStringArrayResult(a, size);
}

inline void il2cpp_codegen_marshal_string_array_out(char** nativeArray, Il2CppCodeGenArray* managedArray)
{
	il2cpp::vm::PlatformInvoke::MarshalStringArrayOut(nativeArray, (Il2CppArray*)managedArray);
}

inline Il2CppCodeGenArray* il2cpp_codegen_marshal_wstring_array_result(Il2CppChar** a, size_t size)
{
	return (Il2CppCodeGenArray*)il2cpp::vm::PlatformInvoke::MarshalWStringArrayResult(a, size);
}

inline Il2CppCodeGenArray* il2cpp_codegen_marshal_bstring_array_result(Il2CppChar** a, size_t size)
{
	return (Il2CppCodeGenArray*)il2cpp::vm::PlatformInvoke::MarshalBStringArrayResult(a, size);
}

inline void il2cpp_codegen_marshal_wstring_array_out(Il2CppChar** nativeArray, Il2CppCodeGenArray* managedArray)
{
	il2cpp::vm::PlatformInvoke::MarshalWStringArrayOut(nativeArray, (Il2CppArray*)managedArray);
}

inline void il2cpp_codegen_marshal_bstring_array_out(Il2CppChar** nativeArray, Il2CppCodeGenArray* managedArray)
{
	il2cpp::vm::PlatformInvoke::MarshalBStringArrayOut(nativeArray, (Il2CppArray*)managedArray);
}

inline void il2cpp_codegen_marshal_free(void* ptr)
{
	il2cpp::vm::PlatformInvoke::MarshalFree(ptr);
}

inline void il2cpp_codegen_marshal_free_string_array(void** a, size_t arrayLength)
{
	il2cpp::vm::PlatformInvoke::MarshalFreeStringArray(a, arrayLength);
}

inline void il2cpp_codegen_marshal_free_bstring_array(Il2CppChar** a, size_t arrayLength)
{
	il2cpp::vm::PlatformInvoke::MarshalFreeBStringArray(a, arrayLength);
}

inline Il2CppMethodPointer il2cpp_codegen_marshal_delegate(Il2CppCodeGenMulticastDelegate* d)
{
	return (Il2CppMethodPointer)il2cpp::vm::PlatformInvoke::MarshalDelegate((Il2CppDelegate*)d).m_value;
}

template <typename T>
inline T* il2cpp_codegen_marshal_function_ptr_to_delegate(Il2CppMethodPointer functionPtr, Il2CppClass* delegateType)
{
	return (T*)il2cpp::vm::PlatformInvoke::MarshalFunctionPointerToDelegate(reinterpret_cast<void*>(functionPtr), delegateType);
}

inline void il2cpp_codegen_marshal_store_last_error()
{
	il2cpp::vm::LastError::StoreLastError();
}

class il2cpp_native_wrapper_vm_thread_attacher
{
public:
	il2cpp_native_wrapper_vm_thread_attacher() :
		_threadWasAttached(false)
	{
		if (il2cpp::vm::Thread::Current() == NULL)
		{
			il2cpp::vm::Thread::Attach(il2cpp::vm::Domain::GetRoot());
			_threadWasAttached = true;
		}
	}

	~il2cpp_native_wrapper_vm_thread_attacher()
	{
		if (_threadWasAttached)
			il2cpp::vm::Thread::Detach(il2cpp::vm::Thread::Current());
	}

private:
	bool _threadWasAttached;
};

#if _DEBUG
inline void il2cpp_codegen_check_marshalling_allocations()
{
	if (il2cpp::vm::MarshalAlloc::HasUnfreedAllocations())
		il2cpp::vm::Exception::Raise(il2cpp::vm::Exception::GetInvalidOperationException("Error in marshaling allocation. Some memory has been leaked."));
}

inline void il2cpp_codegen_clear_all_tracked_marshalling_allocations()
{
	il2cpp::vm::MarshalAlloc::ClearAllTrackedAllocations();
}
#endif

inline void NullCheck (void* this_ptr)
{
	if(this_ptr != NULL)
		return;

	il2cpp::vm::Exception::RaiseNullReferenceException ();
	il2cpp_codegen_no_return();
}

inline void DivideByZeroCheck(int64_t denominator)
{
	if (denominator != 0)
		return;

	il2cpp::vm::Exception::RaiseDivideByZeroException();
	il2cpp_codegen_no_return();
}

inline void Initobj (Il2CppClass* type, void* data)
{
	if (type->valuetype)
		memset (data, 0, type->instance_size - sizeof (Il2CppObject));
	else
		*static_cast<Il2CppObject**> (data) = NULL;
}

inline bool MethodIsStatic(const MethodInfo* method)
{
	return !il2cpp::vm::Method::IsInstance(method);
}

inline bool MethodHasParameters(const MethodInfo* method)
{
	return il2cpp::vm::Method::GetParamCount(method) != 0;
}

#define IL2CPP_RUNTIME_CLASS_INIT(klass) do { if((klass)->has_cctor && !(klass)->cctor_finished) il2cpp::vm::Runtime::ClassInit ((klass)); } while (0)

// generic sharing
#define IL2CPP_RGCTX_DATA(rgctxVar,index) (InitializedTypeInfo(rgctxVar[index].klass))
#define IL2CPP_RGCTX_SIZEOF(rgctxVar,index) (il2cpp_codegen_sizeof(IL2CPP_RGCTX_DATA(rgctxVar, index)))
#define IL2CPP_RGCTX_TYPE(rgctxVar,index) (rgctxVar[index].type)
#define IL2CPP_RGCTX_METHOD_INFO(rgctxVar,index) (rgctxVar[index].method)
#define IL2CPP_RGCTX_FIELD_INFO(Il2CppClass,index) ((Il2CppClass)->fields+index)

inline void ArrayElementTypeCheck(Il2CppCodeGenArray* array, void* value)
{
	if (value != NULL && !il2cpp_codegen_class_is_assignable_from(((Il2CppArray*)array)->klass->element_class, ((Il2CppObject*)value)->klass))
		il2cpp_codegen_raise_exception(il2cpp_codegen_get_array_type_mismatch_exception());
}

inline const MethodInfo* GetVirtualMethodInfo (Il2CppCodeGenObject* pThis, Il2CppMethodSlot slot)
{
	if (!pThis)
		il2cpp::vm::Exception::RaiseNullReferenceException();

	return ((Il2CppObject*)pThis)->klass->vtable[slot].method;
}

inline const MethodInfo* GetInterfaceMethodInfo (Il2CppCodeGenObject* pThis, Il2CppMethodSlot slot, Il2CppClass* declaringInterface)
{
	if (!pThis)
		il2cpp::vm::Exception::RaiseNullReferenceException();

	Il2CppClass* typeInfo = ((Il2CppObject*)pThis)->klass;
	int32_t itf_offset = il2cpp::vm::Class::GetInterfaceOffset(typeInfo, declaringInterface);
	return typeInfo->vtable[itf_offset + slot].method;
}

#if IL2CPP_COMPILER_MSVC
#define STDCALL __stdcall
#define CDECL __cdecl
#define DEFAULT_CALL STDCALL
#else
#define STDCALL
#define CDECL
#define DEFAULT_CALL
#endif

#if defined(__ARMCC_VERSION)
inline double bankers_round(double x)
{
   return __builtin_round(x);
}

inline float bankers_roundf(float x)
{
   return __builtin_roundf(x);
}
#else
inline double bankers_round(double x)
{
	double integerPart;
	if (x >= 0.0)
	{
		if (modf(x, &integerPart) == 0.5)
			return (int64_t)integerPart % 2 == 0 ? integerPart : integerPart + 1.0;
		return floor(x + 0.5);
	}
	else
	{
		if (modf(x, &integerPart) == -0.5)
			return (int64_t)integerPart % 2 == 0 ? integerPart : integerPart - 1.0;
		return	ceil(x - 0.5);
	}
}

inline float bankers_roundf(float x)
{
	double integerPart;
	if (x >= 0.0f)
	{
		if (modf(x, &integerPart) == 0.5)
			return (int64_t)integerPart % 2 == 0 ? (float)integerPart : (float)integerPart + 1.0f;
		return floorf(x + 0.5f);
	}
	else
	{
		if (modf(x, &integerPart) == -0.5)
			return (int64_t)integerPart % 2 == 0 ? (float)integerPart : (float)integerPart - 1.0f;
		return	ceilf(x - 0.5f);
	}
}
#endif

// returns true if overflow occurs
inline bool il2cpp_codegen_check_mul_overflow_i64(int64_t a, int64_t b, int64_t imin, int64_t imax)
{
	// TODO: use a better algorithm without division
	uint64_t ua = (uint64_t) llabs(a);
	uint64_t ub = (uint64_t) llabs(b);

	uint64_t c;
	if ((a > 0 && b > 0) || (a <= 0 && b <= 0))
		c = (uint64_t) llabs(imax);
	else
		c = (uint64_t) llabs(imin);
	
	return ua != 0 && ub > c / ua;
}

inline void il2cpp_codegen_memory_barrier()
{
	il2cpp::vm::Thread::MemoryBarrier();
}

inline void il2cpp_codegen_initialize_method (uint32_t index)
{
	il2cpp::vm::MetadataCache::InitializeMethodMetadata (index);
}

inline bool il2cpp_codegen_type_implements_virtual_method(Il2CppClass* type, int32_t slot)
{
	return type->vtable[slot].method->declaring_type == type;
}

inline Il2CppCodeGenMethodBase* il2cpp_codegen_get_method_object(const MethodInfo* methodInfo)
{
	return (Il2CppCodeGenMethodBase*)il2cpp::vm::Reflection::GetMethodObject(methodInfo, methodInfo->declaring_type);
}

inline Il2CppCodeGenType* il2cpp_codegen_get_type(Il2CppMethodPointer getTypeFunction, Il2CppCodeGenString* typeName, const char* assemblyName)
{
	typedef Il2CppCodeGenType* (*getTypeFuncType)(Il2CppCodeGenObject*, Il2CppCodeGenString*, const MethodInfo*);
	Il2CppString* assemblyQualifiedTypeName = il2cpp::vm::Type::AppendAssemblyNameIfNecessary((Il2CppString*)typeName, assemblyName);

	// Try to find the type using a hint about about calling assembly. If it is not found, fall back to calling GetType without the hint.
	Il2CppCodeGenType* type = ((getTypeFuncType)getTypeFunction)(NULL, (Il2CppCodeGenString*)assemblyQualifiedTypeName, NULL);
	if (type == NULL)
		return ((getTypeFuncType)getTypeFunction)(NULL, typeName, NULL);
	return type;
}

inline Il2CppCodeGenType* il2cpp_codegen_get_type(Il2CppMethodPointer getTypeFunction, Il2CppCodeGenString* typeName, bool throwOnError, const char* assemblyName)
{
	typedef Il2CppCodeGenType* (*getTypeFuncType)(Il2CppCodeGenObject*, Il2CppCodeGenString*, bool, const MethodInfo*);
	Il2CppString* assemblyQualifiedTypeName = il2cpp::vm::Type::AppendAssemblyNameIfNecessary((Il2CppString*)typeName, assemblyName);

	// Try to find the type using a hint about about calling assembly. If it is not found, fall back to calling GetType without the hint.
	Il2CppCodeGenType* type = ((getTypeFuncType)getTypeFunction)(NULL, (Il2CppCodeGenString*)assemblyQualifiedTypeName, throwOnError, NULL);
	if (type == NULL)
		return ((getTypeFuncType)getTypeFunction)(NULL, typeName, throwOnError, NULL);
	return type;
}

inline Il2CppCodeGenType* il2cpp_codegen_get_type(Il2CppMethodPointer getTypeFunction, Il2CppCodeGenString* typeName, bool throwOnError, bool ignoreCase, const char* assemblyName)
{
	typedef Il2CppCodeGenType* (*getTypeFuncType)(Il2CppCodeGenObject*, Il2CppCodeGenString*, bool, bool, const MethodInfo*);
	Il2CppString* assemblyQualifiedTypeName = il2cpp::vm::Type::AppendAssemblyNameIfNecessary((Il2CppString*)typeName, assemblyName);
	// Try to find the type using a hint about about calling assembly. If it is not found, fall back to calling GetType without the hint.

	Il2CppCodeGenType* type = ((getTypeFuncType)getTypeFunction)(NULL, (Il2CppCodeGenString*)assemblyQualifiedTypeName, throwOnError, ignoreCase, NULL);
	if (type == NULL)
		return ((getTypeFuncType)getTypeFunction)(NULL, typeName, throwOnError, ignoreCase, NULL);
	return type;
}

// Atomic

inline void* il2cpp_codegen_atomic_compare_exchange_pointer(void* volatile* dest, void* exchange, void* comparand)
{
	return il2cpp::vm::Atomic::CompareExchangePointer(dest, exchange, comparand);
}

template <typename T>
inline T* il2cpp_codegen_atomic_compare_exchange_pointer(T* volatile* dest, T* newValue, T* oldValue)
{
	return il2cpp::vm::Atomic::CompareExchangePointer(dest, newValue, oldValue);
}

// COM

inline void il2cpp_codegen_com_marshal_variant(Il2CppObject* obj, Il2CppVariant* variant)
{
	il2cpp::vm::COM::MarshalVariant(obj, variant);
}

inline Il2CppObject* il2cpp_codegen_com_marshal_variant_result(Il2CppVariant* variant)
{
	return il2cpp::vm::COM::MarshalVariantResult(variant);
}

inline void il2cpp_codegen_com_destroy_variant(Il2CppVariant* variant)
{
	il2cpp::vm::COM::DestroyVariant(variant);
}

inline Il2CppSafeArray* il2cpp_codegen_com_marshal_safe_array(Il2CppChar type, Il2CppArray* managedArray)
{
	return il2cpp::vm::COM::MarshalSafeArray(type, managedArray);
}

inline Il2CppArray* il2cpp_codegen_com_marshal_safe_array_result(Il2CppChar variantType, Il2CppClass* type, Il2CppSafeArray* safeArray)
{
	return il2cpp::vm::COM::MarshalSafeArrayResult(variantType, type, safeArray);
}

inline Il2CppSafeArray* il2cpp_codegen_com_marshal_safe_array_bstring(Il2CppArray* managedArray)
{
	return il2cpp::vm::COM::MarshalSafeArrayBString((Il2CppArray*)managedArray);
}

inline Il2CppArray* il2cpp_codegen_com_marshal_safe_array_bstring_result(Il2CppClass* type, Il2CppSafeArray* safeArray)
{
	return il2cpp::vm::COM::MarshalSafeArrayBStringResult(type, safeArray);
}

inline void il2cpp_codegen_com_destroy_safe_array(Il2CppSafeArray* safeArray)
{
	il2cpp::vm::COM::DestroySafeArray(safeArray);
}

inline void il2cpp_codegen_com_initialize_object(Il2CppComObject* rcw, const Il2CppGuid& clsid)
{
	il2cpp::vm::RCW::Initialize(rcw, clsid);
}

inline Il2CppIUnknown* il2cpp_codegen_com_query_interface(Il2CppComObject* rcw, const Il2CppGuid& iid)
{
	return il2cpp::vm::RCW::QueryInterface(rcw, iid, true);
}

inline Il2CppObject* il2cpp_codegen_com_create_rcw(Il2CppIUnknown* unknown)
{
	return il2cpp::vm::RCW::Create(unknown);
}

inline void il2cpp_codegen_il2cpp_com_object_cleanup(Il2CppComObject* rcw)
{
	il2cpp::vm::RCW::Cleanup(rcw);
}

inline Il2CppIUnknown* il2cpp_codegen_com_create_ccw(Il2CppObject* obj, const Il2CppGuid& iid)
{
	return il2cpp::vm::CCW::Create((Il2CppObject*)obj, iid);
}

inline Il2CppCodeGenIntPtr il2cpp_codegen_com_get_iunknown_for_object(Il2CppObject* obj)
{
	Il2CppCodeGenIntPtr result;
	result.set_m_value_0(il2cpp::vm::CCW::Create((Il2CppObject*)obj, Il2CppIUnknown::IID));
	return result;
}

inline void il2cpp_codegen_com_raise_exception(il2cpp_hresult_t hr)
{
	il2cpp::vm::Exception::Raise(hr);
}

inline void il2cpp_codegen_com_raise_exception_if_failed(il2cpp_hresult_t hr)
{
	il2cpp::vm::Exception::RaiseIfFailed(hr);
}

inline Il2CppIActivationFactory* il2cpp_codegen_windows_runtime_get_activation_factory(const il2cpp::utils::StringView<Il2CppNativeChar>& runtimeClassName)
{
	return il2cpp::vm::WindowsRuntime::GetActivationFactory(runtimeClassName);
}

template <typename T>
inline Il2CppObject* il2cpp_codegen_fake_box(T* ptrToValueType)
{
	return reinterpret_cast<Il2CppObject*>(ptrToValueType) - 1;
}

// Exception support macros
#define IL2CPP_LEAVE(Offset, Target) \
	__leave_target = Offset; \
	goto Target;

#define IL2CPP_END_FINALLY(Id) \
	goto __CLEANUP_ ## Id;

#define IL2CPP_CLEANUP(Id) \
	__CLEANUP_ ## Id:

#define IL2CPP_RETHROW_IF_UNHANDLED(ExcType) \
	if(__last_unhandled_exception) { \
		ExcType _tmp_exception_local = __last_unhandled_exception; \
		__last_unhandled_exception = 0; \
		il2cpp_codegen_raise_exception(_tmp_exception_local); \
		}

#define IL2CPP_JUMP_TBL(Offset, Target) \
	if(__leave_target == Offset) { \
		__leave_target = 0; \
		goto Target; \
		}

#define IL2CPP_END_CLEANUP(Offset, Target) \
	if(__leave_target == Offset) \
		goto Target;

#define IL2CPP_RAISE_MANAGED_EXCEPTION(message)\
	do {\
		il2cpp_codegen_raise_exception((Il2CppCodeGenException*)message);\
		il2cpp_codegen_no_return();\
	} while (0)


template <typename T>
inline void Il2CppCodeGenWriteBarrier(T** targetAddress, T* object)
{
	// TODO
}
