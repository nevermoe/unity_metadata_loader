#pragma once

#if RUNTIME_MONO

#include "il2cpp-metadata.h"
#include <vector>

struct MethodInfo;
struct VirtualInvokeData;
struct Il2CppInteropData;

void il2cpp_mono_method_initialize_function_pointers(MonoMethod* method, MonoError* error);
MonoAssembly* il2cpp_mono_assembly_from_index(AssemblyIndex index);
void* il2cpp_mono_class_rgctx(MonoClass* klass, Il2CppRGCTXDataType rgctxType, int rgctxIndex, bool useSharedVersion);
void* il2cpp_mono_method_rgctx(MonoMethod* method, Il2CppRGCTXDataType rgctxType, int rgctxIndex, bool useSharedVersion);
MonoMethod* il2cpp_mono_get_virtual_target_method(MonoMethod* method, MonoObject* obj);
void il2cpp_mono_get_invoke_data(MonoMethod* method, void* obj, VirtualInvokeData* invokeData);
void il2cpp_mono_get_virtual_invoke_data(MonoMethod* method, void* obj, VirtualInvokeData* invokeData);
void il2cpp_mono_get_interface_invoke_data(MonoMethod* method, void* obj, VirtualInvokeData* invokeData);
void il2cpp_mono_initialize_method_metadata(uint32_t index);
void il2cpp_mono_raise_execution_engine_exception_if_method_is_not_found(MonoMethod* method);
std::vector<uintptr_t> il2cpp_to_mono_array_dimensions(il2cpp_array_size_t* il2cppDimensions, int32_t numberOfElements);
Il2CppAsyncResult* il2cpp_mono_delegate_begin_invoke(Il2CppDelegate* delegate, void** params, Il2CppDelegate* asyncCallback, MonoObject* state);
MonoObject* il2cpp_mono_delegate_end_invoke(Il2CppAsyncResult* asyncResult, void **out_args);
MonoArray* MonoArrayNew(MonoClass* elementType, uintptr_t length);
MonoMethod* MethodFromIndex(MethodIndex index);
MonoMethod* GenericMethodFromIndex(MethodIndex index);
const Il2CppInteropData* FindInteropDataFor(MonoClass* klass);
void RuntimeInit(MonoClass* klass);
std::string il2cpp_mono_format_exception(const MonoException *exc);
void initialize_interop_data_map();
bool il2cpp_mono_error_ok(MonoError *error);

#if IL2CPP_ENABLE_NATIVE_STACKTRACES
struct MethodDefinitionKey
{
    Il2CppMethodPointer method;
    MethodIndex methodIndex;
    bool isGeneric;
};

void RegisterAllManagedMethods();
#endif // IL2CPP_ENABLE_NATIVE_STACKTRACES

#endif
