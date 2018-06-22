#pragma once

#include "il2cpp-codegen-common.h"
#include "utils/Il2CppHStringReference.h"
#include "utils/RegisterRuntimeInitializeAndCleanup.h"

#include "metadata/GenericMethod.h"
#include "vm/Array.h"
#include "vm/Assembly.h"
#include "vm/Atomic.h"
#include "vm/CCW.h"
#include "vm/Class.h"
#include "vm/COM.h"
#include "vm/Domain.h"
#include "vm/Exception.h"
#include "vm/InternalCalls.h"
#include "vm/LastError.h"
#include "vm/MarshalAlloc.h"
#include "vm/MetadataCache.h"
#include "vm/Method.h"
#include "vm/Object.h"
#include "vm/Profiler.h"
#include "vm/RCW.h"
#include "vm/Reflection.h"
#include "vm/Runtime.h"
#include "vm/String.h"
#include "vm/Thread.h"
#include "vm/ThreadPool.h"
#include "vm/Type.h"
#include "vm/WindowsRuntime.h"
#include "vm/ThreadPoolMs.h"

#ifdef _MSC_VER
#define IL2CPP_DISABLE_OPTIMIZATIONS __pragma(optimize("", off))
#define IL2CPP_ENABLE_OPTIMIZATIONS __pragma(optimize("", on))
#else
#define IL2CPP_DISABLE_OPTIMIZATIONS
#define IL2CPP_ENABLE_OPTIMIZATIONS
#endif

struct ProfilerMethodSentry
{
    ProfilerMethodSentry(const RuntimeMethod* method)
#if IL2CPP_ENABLE_PROFILER
        : m_method(method)
#endif
    {
#if IL2CPP_ENABLE_PROFILER
        il2cpp::vm::Profiler::MethodEnter(m_method);
#endif
    }

    ~ProfilerMethodSentry()
    {
#if IL2CPP_ENABLE_PROFILER
        il2cpp::vm::Profiler::MethodExit(m_method);
#endif
    }

private:
    const RuntimeMethod* m_method;
};

struct StackTraceSentry
{
    StackTraceSentry(const RuntimeMethod* method) : m_method(method)
    {
        Il2CppStackFrameInfo frame_info;

        frame_info.method = method;

#if IL2CPP_DEBUGGER_ENABLED
        frame_info.id = -1;
        frame_info.il_offset = 0;
        frame_info.type = FRAME_TYPE_MANAGED;
#endif
        il2cpp::vm::StackTrace::PushFrame(frame_info);
    }

#if IL2CPP_DEBUGGER_ENABLED
    StackTraceSentry(const RuntimeMethod* method, void* this_ptr, void **params, int32_t params_count, void **locals, int32_t locals_count) : m_method(method)
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
        il2cpp::vm::StackTrace::PushFrame(frame_info);
    }

#endif

    ~StackTraceSentry()
    {
        il2cpp::vm::StackTrace::PopFrame();
    }

private:
    const RuntimeMethod* m_method;
};

template<typename T>
struct Il2CppFakeBox : RuntimeObject
{
    T m_Value;

    Il2CppFakeBox(RuntimeClass* boxedType, T* value)
    {
        klass = boxedType;
        monitor = NULL;
        m_Value = *value;
    }
};

// TODO: This file should contain all the functions and type declarations needed for the generated code.
// Hopefully, we stop including everything in the generated code and know exactly what dependencies we have.
// Note that all parameter and return types should match the generated types not the runtime types.

inline void il2cpp_codegen_register(const Il2CppCodeRegistration* const codeRegistration, const Il2CppMetadataRegistration* const metadataRegistration, const Il2CppCodeGenOptions* const codeGenOptions)
{
    il2cpp::vm::MetadataCache::Register(codeRegistration, metadataRegistration, codeGenOptions);
}

#include "GeneratedCodeGen.h"

// type registration

inline void* il2cpp_codegen_get_thread_static_data(RuntimeClass* klass)
{
    return il2cpp::vm::Thread::GetThreadStaticData(klass->thread_static_fields_offset);
}

inline Il2CppCodeGenString* il2cpp_codegen_string_new_wrapper(const char* str)
{
    return (Il2CppCodeGenString*)il2cpp::vm::String::NewWrapper(str);
}

inline Il2CppCodeGenString* il2cpp_codegen_string_new_utf16(const il2cpp::utils::StringView<Il2CppChar>& str)
{
    return (Il2CppCodeGenString*)il2cpp::vm::String::NewUtf16(str.Str(), static_cast<int32_t>(str.Length()));
}

inline Il2CppCodeGenType* il2cpp_codegen_type_get_object(const RuntimeType* type)
{
    return (Il2CppCodeGenType*)il2cpp::vm::Reflection::GetTypeObject(type);
}

inline NORETURN void il2cpp_codegen_raise_exception(Il2CppCodeGenException *ex)
{
    il2cpp::vm::Exception::Raise((RuntimeException*)ex);
}

inline void il2cpp_codegen_raise_execution_engine_exception_if_method_is_not_found(const RuntimeMethod* method)
{
    il2cpp::vm::Runtime::RaiseExecutionEngineExceptionIfMethodIsNotFound(method);
}

inline void il2cpp_codegen_raise_execution_engine_exception(const RuntimeMethod* method)
{
    il2cpp::vm::Runtime::AlwaysRaiseExecutionEngineException(method);
}

inline void il2cpp_codegen_raise_out_of_memory_exception()
{
    il2cpp::vm::Exception::RaiseOutOfMemoryException();
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

inline Il2CppCodeGenException* il2cpp_codegen_get_invalid_cast_exception(const char* msg)
{
    return (Il2CppCodeGenException*)il2cpp::vm::Exception::GetInvalidCastException(msg);
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

inline RuntimeClass* il2cpp_codegen_object_class(RuntimeObject* obj)
{
    return obj->klass;
}

// OpCode.IsInst

inline RuntimeObject* IsInst(RuntimeObject *obj, RuntimeClass* targetType)
{
    return il2cpp::vm::Object::IsInst(obj, targetType);
}

inline RuntimeObject* IsInstSealed(RuntimeObject *obj, RuntimeClass* targetType)
{
#if IL2CPP_DEBUG
    IL2CPP_ASSERT((targetType->flags & TYPE_ATTRIBUTE_SEALED) != 0);
    IL2CPP_ASSERT((targetType->flags & TYPE_ATTRIBUTE_INTERFACE) == 0);
#endif
    if (!obj)
        return NULL;

    // optimized version to compare sealed classes
    return (obj->klass == targetType ? obj : NULL);
}

inline RuntimeObject* IsInstClass(RuntimeObject *obj, RuntimeClass* targetType)
{
#if IL2CPP_DEBUG
    IL2CPP_ASSERT((targetType->flags & TYPE_ATTRIBUTE_INTERFACE) == 0);
#endif
    if (!obj)
        return NULL;

    // optimized version to compare classes
    return il2cpp::vm::Class::HasParentUnsafe(obj->klass, targetType) ? obj : NULL;
}

// OpCode.Castclass

NORETURN inline void RaiseInvalidCastException(RuntimeObject* obj, RuntimeClass* targetType)
{
    std::string exceptionMessage = il2cpp::utils::Exception::FormatInvalidCastException(obj->klass->element_class, targetType);
    RuntimeException* exception = il2cpp::vm::Exception::GetInvalidCastException(exceptionMessage.c_str());
    il2cpp::vm::Exception::Raise(exception);
}

inline RuntimeObject* Castclass(RuntimeObject *obj, RuntimeClass* targetType)
{
    if (!obj)
        return NULL;

    RuntimeObject* result = il2cpp::vm::Object::IsInst(obj, targetType);
    if (result)
        return result;

    RaiseInvalidCastException(obj, targetType);
    return NULL;
}

inline RuntimeObject* CastclassSealed(RuntimeObject *obj, RuntimeClass* targetType)
{
    if (!obj)
        return NULL;

    RuntimeObject* result = IsInstSealed(obj, targetType);
    if (result)
        return result;

    RaiseInvalidCastException(obj, targetType);
    return NULL;
}

inline RuntimeObject* CastclassClass(RuntimeObject *obj, RuntimeClass* targetType)
{
    if (!obj)
        return NULL;

    RuntimeObject* result = IsInstClass(obj, targetType);
    if (result)
        return result;

    RaiseInvalidCastException(obj, targetType);
    return NULL;
}

inline void NullCheck(void* this_ptr)
{
    if (this_ptr != NULL)
        return;

    il2cpp::vm::Exception::RaiseNullReferenceException();
#if !IL2CPP_TARGET_IOS
    il2cpp_codegen_no_return();
#endif
}

// OpCode.Box

inline RuntimeObject* Box(RuntimeClass* type, void* data)
{
    return il2cpp::vm::Object::Box(type, data);
}

// OpCode.UnBox

inline void* UnBox(RuntimeObject* obj)
{
    NullCheck(obj);
    return il2cpp::vm::Object::Unbox(obj);
}

inline void* UnBox(RuntimeObject* obj, RuntimeClass* expectedBoxedClass)
{
    NullCheck(obj);

    if (obj->klass->element_class == expectedBoxedClass->element_class)
        return il2cpp::vm::Object::Unbox(obj);

    RaiseInvalidCastException(obj, expectedBoxedClass);
    return NULL;
}

inline void UnBoxNullable(RuntimeObject* obj, RuntimeClass* expectedBoxedClass, void* storage)
{
    // We only need to do type checks if obj is not null
    // Unboxing null nullable is perfectly valid and returns an instance that has no value
    if (obj != NULL)
    {
        if (obj->klass->element_class != expectedBoxedClass->element_class)
            RaiseInvalidCastException(obj, expectedBoxedClass);
    }

    il2cpp::vm::Object::UnboxNullable(obj, expectedBoxedClass, storage);
}

inline uint32_t il2cpp_codegen_sizeof(RuntimeClass* klass)
{
    if (!klass->valuetype)
    {
        return sizeof(void*);
    }

    return il2cpp::vm::Class::GetInstanceSize(klass) - sizeof(RuntimeObject);
}

FORCE_INLINE const VirtualInvokeData& il2cpp_codegen_get_virtual_invoke_data(Il2CppMethodSlot slot, const RuntimeObject* obj)
{
    Assert(slot != 65535 && "il2cpp_codegen_get_virtual_invoke_data got called on a non-virtual method");
    return obj->klass->vtable[slot];
}

FORCE_INLINE const VirtualInvokeData& il2cpp_codegen_get_interface_invoke_data(Il2CppMethodSlot slot, const RuntimeObject* obj, const RuntimeClass* declaringInterface)
{
    Assert(slot != 65535 && "il2cpp_codegen_get_interface_invoke_data got called on a non-virtual method");
    return il2cpp::vm::Class::GetInterfaceInvokeDataFromVTable(obj, declaringInterface, slot);
}

FORCE_INLINE const RuntimeMethod* il2cpp_codegen_get_generic_virtual_method(const RuntimeMethod* method, const RuntimeObject* obj)
{
    uint16_t slot = method->slot;
    const RuntimeMethod* methodDefinition = obj->klass->vtable[slot].method;
    return il2cpp::vm::Runtime::GetGenericVirtualMethod(methodDefinition, method);
}

FORCE_INLINE void il2cpp_codegen_get_generic_virtual_invoke_data(const RuntimeMethod* method, const RuntimeObject* obj, VirtualInvokeData* invokeData)
{
    const RuntimeMethod* targetRuntimeMethod = il2cpp_codegen_get_generic_virtual_method(method, obj);
#if IL2CPP_DEBUG
    IL2CPP_ASSERT(targetRuntimeMethod);
#endif

    invokeData->methodPtr = targetRuntimeMethod->methodPointer;
    invokeData->method = targetRuntimeMethod;
}

FORCE_INLINE const RuntimeMethod* il2cpp_codegen_get_generic_interface_method(const RuntimeMethod* method, const RuntimeObject* obj)
{
    const RuntimeMethod* methodDefinition = il2cpp::vm::Class::GetInterfaceInvokeDataFromVTable(obj, method->declaring_type, method->slot).method;
    return il2cpp::vm::Runtime::GetGenericVirtualMethod(methodDefinition, method);
}

FORCE_INLINE void il2cpp_codegen_get_generic_interface_invoke_data(const RuntimeMethod* method, const RuntimeObject* obj, VirtualInvokeData* invokeData)
{
    const RuntimeMethod* targetRuntimeMethod = il2cpp_codegen_get_generic_interface_method(method, obj);

#if IL2CPP_DEBUG
    IL2CPP_ASSERT(targetRuntimeMethod);
#endif

    invokeData->methodPtr = targetRuntimeMethod->methodPointer;
    invokeData->method = targetRuntimeMethod;
}

#include "GeneratedVirtualInvokers.h"
#include "GeneratedInterfaceInvokers.h"
#include "GeneratedGenericVirtualInvokers.h"
#include "GeneratedGenericInterfaceInvokers.h"

// OpCode.Ldtoken

inline Il2CppCodeGenRuntimeTypeHandle LoadTypeToken(const RuntimeType* ptr)
{
    Il2CppCodeGenIntPtr intptr;
    intptr.set_m_value_0(const_cast<RuntimeType*>(ptr));

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

    IL2CPP_ASSERT(false && "__arglist usage not supported.");
    return handle;
}

inline Il2CppCodeGenRuntimeMethodHandle LoadMethodToken(const RuntimeMethod* ptr)
{
    Il2CppCodeGenIntPtr intptr;
    intptr.set_m_value_0(const_cast<RuntimeMethod*>(ptr));

    Il2CppCodeGenRuntimeMethodHandle handle;
    handle.set_value_0(intptr);

    return handle;
}

inline RuntimeClass* InitializedTypeInfo(RuntimeClass* klass)
{
    il2cpp::vm::Class::Init(klass);
    return klass;
}

inline RuntimeClass* il2cpp_codegen_class_from_type(const RuntimeType *type)
{
    return InitializedTypeInfo(il2cpp::vm::Class::FromIl2CppType(type));
}

inline void* InterlockedExchangeImplRef(void** location, void* value)
{
    return il2cpp::icalls::mscorlib::System::Threading::Interlocked::ExchangePointer(location, value);
}

template<typename T>
inline T InterlockedCompareExchangeImpl(T* location, T value, T comparand)
{
    return (T)il2cpp::icalls::mscorlib::System::Threading::Interlocked::CompareExchange_T((void**)location, value, comparand);
}

template<typename T>
inline T InterlockedExchangeImpl(T* location, T value)
{
    return (T)InterlockedExchangeImplRef((void**)location, value);
}

inline void il2cpp_codegen_memory_barrier()
{
    il2cpp::vm::Thread::MemoryBarrier();
}

inline void ArrayGetGenericValueImpl(RuntimeArray* thisPtr, int32_t pos, void* value)
{
    memcpy(value, ((uint8_t*)thisPtr) + sizeof(RuntimeArray) + pos * thisPtr->klass->element_size, thisPtr->klass->element_size);
}

inline void ArraySetGenericValueImpl(RuntimeArray * thisPtr, int32_t pos, void* value)
{
    memcpy(((uint8_t*)thisPtr) + sizeof(RuntimeArray) + pos * thisPtr->klass->element_size, value, thisPtr->klass->element_size);
}

inline RuntimeArray* SZArrayNew(RuntimeClass* arrayType, uint32_t length)
{
    il2cpp::vm::Class::Init(arrayType);
    return il2cpp::vm::Array::NewSpecific(arrayType, length);
}

inline RuntimeArray* GenArrayNew(RuntimeClass* arrayType, il2cpp_array_size_t* dimensions)
{
    return il2cpp::vm::Array::NewFull(arrayType, dimensions, NULL);
}

// Performance optimization as detailed here: http://blogs.msdn.com/b/clrcodegeneration/archive/2009/08/13/array-bounds-check-elimination-in-the-clr.aspx
// Since array size is a signed int32_t, a single unsigned check can be performed to determine if index is less than array size.
// Negative indices will map to a unsigned number greater than or equal to 2^31 which is larger than allowed for a valid array.
#define IL2CPP_ARRAY_BOUNDS_CHECK(index, length) \
    do { \
        if (((uint32_t)(index)) >= ((uint32_t)length)) il2cpp::vm::Exception::Raise (il2cpp::vm::Exception::GetIndexOutOfRangeException()); \
    } while (0)

inline bool il2cpp_codegen_class_is_assignable_from(RuntimeClass *klass, RuntimeClass *oklass)
{
    return il2cpp::vm::Class::IsAssignableFrom(klass, oklass);
}

inline RuntimeObject* il2cpp_codegen_object_new(RuntimeClass *klass)
{
    return il2cpp::vm::Object::New(klass);
}

inline Il2CppMethodPointer il2cpp_codegen_resolve_icall(const char* name)
{
    Il2CppMethodPointer method = il2cpp::vm::InternalCalls::Resolve(name);
    if (!method)
    {
        il2cpp::vm::Exception::Raise(il2cpp::vm::Exception::GetMissingMethodException(name));
    }
    return method;
}

template<typename FunctionPointerType, size_t dynamicLibraryLength, size_t entryPointLength>
inline FunctionPointerType il2cpp_codegen_resolve_pinvoke(const Il2CppNativeChar(&nativeDynamicLibrary)[dynamicLibraryLength], const char(&entryPoint)[entryPointLength],
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

template<typename T>
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

inline Il2CppHString il2cpp_codegen_create_hstring(Il2CppCodeGenString* str)
{
    return il2cpp::vm::WindowsRuntime::CreateHString(reinterpret_cast<Il2CppString*>(str));
}

inline Il2CppCodeGenString* il2cpp_codegen_marshal_hstring_result(Il2CppHString hstring)
{
    return reinterpret_cast<Il2CppCodeGenString*>(il2cpp::vm::WindowsRuntime::HStringToManagedString(hstring));
}

inline void il2cpp_codegen_marshal_free_hstring(Il2CppHString hstring)
{
    il2cpp::vm::WindowsRuntime::DeleteHString(hstring);
}

inline void il2cpp_codegen_marshal_free(void* ptr)
{
    il2cpp::vm::PlatformInvoke::MarshalFree(ptr);
}

inline Il2CppMethodPointer il2cpp_codegen_marshal_delegate(Il2CppCodeGenMulticastDelegate* d)
{
    return (Il2CppMethodPointer)il2cpp::vm::PlatformInvoke::MarshalDelegate((Il2CppDelegate*)d).m_value;
}

template<typename T>
inline T* il2cpp_codegen_marshal_function_ptr_to_delegate(Il2CppMethodPointer functionPtr, RuntimeClass* delegateType)
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
struct ScopedMarshallingAllocationFrame
{
    ScopedMarshallingAllocationFrame()
    {
        il2cpp::vm::MarshalAlloc::PushAllocationFrame();
    }

    ~ScopedMarshallingAllocationFrame()
    {
        il2cpp::vm::MarshalAlloc::PopAllocationFrame();
    }
};

struct ScopedMarshallingAllocationCheck
{
    ~ScopedMarshallingAllocationCheck()
    {
        if (il2cpp::vm::MarshalAlloc::HasUnfreedAllocations())
            il2cpp::vm::Exception::Raise(il2cpp::vm::Exception::GetInvalidOperationException("Error in marshaling allocation. Some memory has been leaked."));
    }

private:
    ScopedMarshallingAllocationFrame m_AllocationFrame;
};

struct ScopedMarshalingAllocationClearer
{
    ~ScopedMarshalingAllocationClearer()
    {
        il2cpp::vm::MarshalAlloc::ClearAllTrackedAllocations();
    }

private:
    ScopedMarshallingAllocationFrame m_AllocationFrame;
};

#endif

inline void DivideByZeroCheck(int64_t denominator)
{
    if (denominator != 0)
        return;

    il2cpp::vm::Exception::RaiseDivideByZeroException();
#if !IL2CPP_TARGET_IOS
    il2cpp_codegen_no_return();
#endif
}

inline void Initobj(RuntimeClass* type, void* data)
{
    if (type->valuetype)
        memset(data, 0, type->instance_size - sizeof(RuntimeObject));
    else
        *static_cast<RuntimeObject**>(data) = NULL;
}

inline bool MethodIsStatic(const RuntimeMethod* method)
{
    return !il2cpp::vm::Method::IsInstance(method);
}

inline bool MethodHasParameters(const RuntimeMethod* method)
{
    return il2cpp::vm::Method::GetParamCount(method) != 0;
}

#define IL2CPP_RUNTIME_CLASS_INIT(klass) do { if((klass)->has_cctor && !(klass)->cctor_finished) il2cpp::vm::Runtime::ClassInit ((klass)); } while (0)

// generic sharing
#define IL2CPP_RGCTX_DATA(rgctxVar, index) (InitializedTypeInfo(rgctxVar[index].klass))
#define IL2CPP_RGCTX_SIZEOF(rgctxVar, index) (il2cpp_codegen_sizeof(IL2CPP_RGCTX_DATA(rgctxVar, index)))
#define IL2CPP_RGCTX_TYPE(rgctxVar, index) (rgctxVar[index].type)
#define IL2CPP_RGCTX_METHOD_INFO(rgctxVar, index) (rgctxVar[index].method)
#define IL2CPP_RGCTX_FIELD_INFO(klass, index) ((klass)->fields+index)

inline void ArrayElementTypeCheck(RuntimeArray* array, void* value)
{
    if (value != NULL && il2cpp::vm::Object::IsInst((RuntimeObject*)value, array->klass->element_class) == NULL)
        il2cpp_codegen_raise_exception(il2cpp_codegen_get_array_type_mismatch_exception());
}

inline const RuntimeMethod* GetVirtualMethodInfo(RuntimeObject* pThis, Il2CppMethodSlot slot)
{
    if (!pThis)
        il2cpp::vm::Exception::RaiseNullReferenceException();

    return pThis->klass->vtable[slot].method;
}

inline const RuntimeMethod* GetInterfaceMethodInfo(RuntimeObject* pThis, Il2CppMethodSlot slot, RuntimeClass* declaringInterface)
{
    if (!pThis)
        il2cpp::vm::Exception::RaiseNullReferenceException();

    return il2cpp::vm::Class::GetInterfaceInvokeDataFromVTable(pThis, declaringInterface, slot).method;
}

inline void il2cpp_codegen_initialize_method(uint32_t index)
{
    il2cpp::vm::MetadataCache::InitializeMethodMetadata(index);
}

inline bool il2cpp_codegen_type_implements_virtual_method(RuntimeClass* type, const RuntimeMethod* method)
{
    IL2CPP_ASSERT(il2cpp::vm::Class::IsValuetype(type));
    return method->declaring_type == type;
}

inline Il2CppCodeGenMethodBase* il2cpp_codegen_get_method_object(const RuntimeMethod* method)
{
    if (method->is_inflated)
        method = il2cpp::vm::MetadataCache::GetGenericMethodDefinition(method);
    return (Il2CppCodeGenMethodBase*)il2cpp::vm::Reflection::GetMethodObject(method, method->declaring_type);
}

inline Il2CppCodeGenType* il2cpp_codegen_get_type(Il2CppMethodPointer getTypeFunction, Il2CppCodeGenString* typeName, const char* assemblyName)
{
    typedef Il2CppCodeGenType* (*getTypeFuncType)(RuntimeObject*, Il2CppCodeGenString*, const RuntimeMethod*);
    Il2CppString* assemblyQualifiedTypeName = il2cpp::vm::Type::AppendAssemblyNameIfNecessary((Il2CppString*)typeName, assemblyName);

    // Try to find the type using a hint about about calling assembly. If it is not found, fall back to calling GetType without the hint.
    Il2CppCodeGenType* type = ((getTypeFuncType)getTypeFunction)(NULL, (Il2CppCodeGenString*)assemblyQualifiedTypeName, NULL);
    if (type == NULL)
        return ((getTypeFuncType)getTypeFunction)(NULL, typeName, NULL);
    return type;
}

inline Il2CppCodeGenType* il2cpp_codegen_get_type(Il2CppMethodPointer getTypeFunction, Il2CppCodeGenString* typeName, bool throwOnError, const char* assemblyName)
{
    typedef Il2CppCodeGenType* (*getTypeFuncType)(RuntimeObject*, Il2CppCodeGenString*, bool, const RuntimeMethod*);
    Il2CppString* assemblyQualifiedTypeName = il2cpp::vm::Type::AppendAssemblyNameIfNecessary((Il2CppString*)typeName, assemblyName);

    // Try to find the type using a hint about about calling assembly. If it is not found, fall back to calling GetType without the hint.
    Il2CppCodeGenType* type = ((getTypeFuncType)getTypeFunction)(NULL, (Il2CppCodeGenString*)assemblyQualifiedTypeName, throwOnError, NULL);
    if (type == NULL)
        return ((getTypeFuncType)getTypeFunction)(NULL, typeName, throwOnError, NULL);
    return type;
}

inline Il2CppCodeGenType* il2cpp_codegen_get_type(Il2CppMethodPointer getTypeFunction, Il2CppCodeGenString* typeName, bool throwOnError, bool ignoreCase, const char* assemblyName)
{
    typedef Il2CppCodeGenType* (*getTypeFuncType)(RuntimeObject*, Il2CppCodeGenString*, bool, bool, const RuntimeMethod*);
    Il2CppString* assemblyQualifiedTypeName = il2cpp::vm::Type::AppendAssemblyNameIfNecessary((Il2CppString*)typeName, assemblyName);
    // Try to find the type using a hint about about calling assembly. If it is not found, fall back to calling GetType without the hint.

    Il2CppCodeGenType* type = ((getTypeFuncType)getTypeFunction)(NULL, (Il2CppCodeGenString*)assemblyQualifiedTypeName, throwOnError, ignoreCase, NULL);
    if (type == NULL)
        return ((getTypeFuncType)getTypeFunction)(NULL, typeName, throwOnError, ignoreCase, NULL);
    return type;
}

inline Il2CppCodeGenAssembly* il2cpp_codegen_get_executing_assembly(const RuntimeMethod* method)
{
    return (Il2CppCodeGenAssembly*)il2cpp::vm::Reflection::GetAssemblyObject(il2cpp::vm::MetadataCache::GetAssemblyFromIndex(method->declaring_type->image->assemblyIndex));
}

// Atomic

inline void* il2cpp_codegen_atomic_compare_exchange_pointer(void* volatile* dest, void* exchange, void* comparand)
{
    return il2cpp::vm::Atomic::CompareExchangePointer(dest, exchange, comparand);
}

template<typename T>
inline T* il2cpp_codegen_atomic_compare_exchange_pointer(T* volatile* dest, T* newValue, T* oldValue)
{
    return il2cpp::vm::Atomic::CompareExchangePointer(dest, newValue, oldValue);
}

// COM

inline void il2cpp_codegen_com_marshal_variant(RuntimeObject* obj, Il2CppVariant* variant)
{
    il2cpp::vm::COM::MarshalVariant(obj, variant);
}

inline RuntimeObject* il2cpp_codegen_com_marshal_variant_result(const Il2CppVariant* variant)
{
    return il2cpp::vm::COM::MarshalVariantResult(variant);
}

inline void il2cpp_codegen_com_destroy_variant(Il2CppVariant* variant)
{
    il2cpp::vm::COM::DestroyVariant(variant);
}

inline Il2CppSafeArray* il2cpp_codegen_com_marshal_safe_array(Il2CppChar type, RuntimeArray* managedArray)
{
    return il2cpp::vm::COM::MarshalSafeArray(type, managedArray);
}

inline RuntimeArray* il2cpp_codegen_com_marshal_safe_array_result(Il2CppChar variantType, RuntimeClass* type, Il2CppSafeArray* safeArray)
{
    return il2cpp::vm::COM::MarshalSafeArrayResult(variantType, type, safeArray);
}

inline Il2CppSafeArray* il2cpp_codegen_com_marshal_safe_array_bstring(RuntimeArray* managedArray)
{
    return il2cpp::vm::COM::MarshalSafeArrayBString(managedArray);
}

inline RuntimeArray* il2cpp_codegen_com_marshal_safe_array_bstring_result(RuntimeClass* type, Il2CppSafeArray* safeArray)
{
    return il2cpp::vm::COM::MarshalSafeArrayBStringResult(type, safeArray);
}

inline void il2cpp_codegen_com_destroy_safe_array(Il2CppSafeArray* safeArray)
{
    il2cpp::vm::COM::DestroySafeArray(safeArray);
}

inline void il2cpp_codegen_com_create_instance(const Il2CppGuid& clsid, Il2CppIUnknown** identity)
{
    il2cpp::vm::COM::CreateInstance(clsid, identity);
}

inline void il2cpp_codegen_com_register_rcw(Il2CppComObject* rcw)
{
    il2cpp::vm::RCW::Register(rcw);
}

template<typename T>
inline T* il2cpp_codegen_com_get_or_create_rcw_from_iunknown(Il2CppIUnknown* unknown, RuntimeClass* fallbackClass)
{
    return static_cast<T*>(il2cpp::vm::RCW::GetOrCreateFromIUnknown(unknown, fallbackClass));
}

template<typename T>
inline T* il2cpp_codegen_com_get_or_create_rcw_from_iinspectable(Il2CppIInspectable* unknown, RuntimeClass* fallbackClass)
{
    return static_cast<T*>(il2cpp::vm::RCW::GetOrCreateFromIInspectable(unknown, fallbackClass));
}

template<typename T>
inline T* il2cpp_codegen_com_get_or_create_rcw_for_sealed_class(Il2CppIUnknown* unknown, RuntimeClass* objectClass)
{
    return static_cast<T*>(il2cpp::vm::RCW::GetOrCreateForSealedClass(unknown, objectClass));
}

inline void il2cpp_codegen_il2cpp_com_object_cleanup(Il2CppComObject* rcw)
{
    il2cpp::vm::RCW::Cleanup(rcw);
}

template<typename InterfaceType>
inline InterfaceType* il2cpp_codegen_com_get_or_create_ccw(RuntimeObject* obj)
{
    return static_cast<InterfaceType*>(il2cpp::vm::CCW::GetOrCreate(obj, InterfaceType::IID));
}

inline Il2CppCodeGenIntPtr il2cpp_codegen_com_get_iunknown_for_object(RuntimeObject* obj)
{
    Il2CppCodeGenIntPtr result;
    result.set_m_value_0(il2cpp::vm::CCW::GetOrCreate(obj, Il2CppIUnknown::IID));
    return result;
}

inline void il2cpp_codegen_com_raise_exception_if_failed(il2cpp_hresult_t hr, bool defaultToCOMException)
{
    il2cpp::vm::Exception::RaiseIfFailed(hr, defaultToCOMException);
}

inline RuntimeException* il2cpp_codegen_com_get_exception(il2cpp_hresult_t hr, bool defaultToCOMException)
{
    return il2cpp::vm::Exception::Get(hr, defaultToCOMException);
}

inline RuntimeException* il2cpp_codegen_com_get_exception_for_invalid_iproperty_cast(RuntimeObject* value, const char* a, const char* b)
{
    return il2cpp::vm::CCW::GetIPropertyValueInvalidCast(value, a, b);
}

inline void il2cpp_codegen_store_exception_info(RuntimeException* ex, Il2CppCodeGenString* exceptionString)
{
    il2cpp::vm::Exception::StoreExceptionInfo(ex, reinterpret_cast<Il2CppString*>(exceptionString));
}

inline Il2CppIActivationFactory* il2cpp_codegen_windows_runtime_get_activation_factory(const il2cpp::utils::StringView<Il2CppNativeChar>& runtimeClassName)
{
    return il2cpp::vm::WindowsRuntime::GetActivationFactory(runtimeClassName);
}

// delegate

inline Il2CppAsyncResult* il2cpp_codegen_delegate_begin_invoke(Il2CppDelegate* delegate, void** params, Il2CppDelegate* asyncCallback, RuntimeObject* state)
{
#if NET_4_0
    return il2cpp::vm::ThreadPoolMs::DelegateBeginInvoke(delegate, params, asyncCallback, state);
#else
    return il2cpp::vm::ThreadPool::Queue(delegate, params, asyncCallback, state);
#endif
}

inline RuntimeObject* il2cpp_codegen_delegate_end_invoke(Il2CppAsyncResult* asyncResult, void **out_args)
{
#if NET_4_0
    return il2cpp::vm::ThreadPoolMs::DelegateEndInvoke(asyncResult, out_args);
#else
    return il2cpp::vm::ThreadPool::Wait(asyncResult, out_args);
#endif
}

inline const Il2CppGenericInst* il2cpp_codegen_get_generic_class_inst(RuntimeClass* genericClass)
{
    IL2CPP_ASSERT(genericClass->generic_class);
    return genericClass->generic_class->context.class_inst;
}

inline RuntimeClass* il2cpp_codegen_inflate_generic_class(RuntimeClass* genericClassDefinition, const Il2CppGenericInst* genericInst)
{
    return il2cpp::vm::Class::GetInflatedGenericInstanceClass(genericClassDefinition, genericInst);
}

inline void* il2cpp_codegen_static_fields_for(RuntimeClass* klass)
{
    return klass->static_fields;
}

inline Il2CppMethodPointer il2cpp_codegen_get_method_pointer(const RuntimeMethod* method)
{
    return method->methodPointer;
}

inline const RuntimeType* il2cpp_codegen_method_return_type(const RuntimeMethod* method)
{
    return method->return_type;
}

inline bool il2cpp_codegen_is_import_or_windows_runtime(const RuntimeObject *object)
{
    return object->klass->is_import_or_windows_runtime;
}

inline std::string il2cpp_codegen_format_exception(const RuntimeException* ex)
{
    return il2cpp::utils::Exception::FormatException(ex);
}
