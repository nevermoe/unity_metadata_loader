#pragma once

#include "il2cpp-config.h"

// This file is a compile-time abstraction for VM support needed by code in the os and utils namespaces that is used by both the
// libil2cpp and the libmono runtimes. Code in those namespaces should never depend up on the vm namespace directly.

#if defined(RUNTIME_MONO)
    #include "mono-api.h"
    #include "il2cpp-mono-support.h"
    #include <cassert>
    #define IL2CPP_VM_RAISE_EXCEPTION(exception) mono_raise_exception(exception)
    #define IL2CPP_VM_RAISE_COM_EXCEPTION(hresult, defaultToCOMException) assert(false && "COM exceptions are not implemented yet.")
    #define IL2CPP_VM_RAISE_IF_FAILED(hresult, defaultToCOMException) assert(false && "COM exceptions are not implemented yet.")
    #define IL2CPP_VM_STRING_EMPTY() mono_string_empty(mono_domain_get())
    #define IL2CPP_VM_STRING_NEW_UTF16(value, length) mono_string_new_utf16(mono_domain_get(), value, length)
    #define IL2CPP_VM_STRING_NEW_LEN(value, length) mono_string_new_len(mono_domain_get(), value, length);
    #define IL2CPP_VM_NOT_SUPPORTED(func, reason) mono_raise_exception(mono_get_exception_not_supported(NOTSUPPORTEDICALLMESSAGE ("IL2CPP", #func, #reason)))
    #define IL2CPP_VM_METHOD_METADATA_FROM_INDEX(isGeneric, methodIndex) isGeneric ? GenericMethodFromIndex(methodIndex) : MethodFromIndex(methodIndex)
    #define IL2CPP_VM_SHUTDOWN() do { if (mono_runtime_try_shutdown()) mono_runtime_quit(); } while(0)
    #define IL2CPP_VM_GET_CREATE_CCW_EXCEPTION(ex) NULL
typedef MonoString VmString;
typedef MonoMethod VmMethod;
#elif RUNTIME_NONE // OS layer compiled with no runtime
    #define IL2CPP_VM_RAISE_EXCEPTION(exception) IL2CPP_ASSERT(0 && "This is not implemented wihout a VM runtime backend.")
    #define IL2CPP_VM_RAISE_COM_EXCEPTION(hresult, defaultToCOMException) IL2CPP_ASSERT(0 && "This is not implemented wihout a VM runtime backend.")
    #define IL2CPP_VM_RAISE_IF_FAILED(hresult, defaultToCOMException) IL2CPP_ASSERT(0 && "This is not implemented wihout a VM runtime backend.")
    #define IL2CPP_VM_STRING_EMPTY() NULL
    #define IL2CPP_VM_STRING_NEW_UTF16(value, length) NULL
    #define IL2CPP_VM_STRING_NEW_LEN(value, length) NULL
    #define IL2CPP_VM_NOT_SUPPORTED(func, reason) IL2CPP_ASSERT(0 && "This is not implemented wihout a VM runtime backend.")
    #define IL2CPP_VM_METHOD_METADATA_FROM_INDEX(isGeneric, methodIndex) IL2CPP_ASSERT(0 && "This is not implemented wihout a VM runtime backend.")
    #define IL2CPP_VM_SHUTDOWN() IL2CPP_ASSERT(0 && "This is not implemented wihout a VM runtime backend.")
    #define IL2CPP_VM_GET_CREATE_CCW_EXCEPTION(ex) NULL
#else // Assume the libil2cpp runtime
    #include "vm/Exception.h"
    #include "vm/MetadataCache.h"
    #include "vm/StackTrace.h"
    #include "vm/String.h"
    #define IL2CPP_VM_RAISE_EXCEPTION(exception) il2cpp::vm::Exception::Raise(exception)
    #define IL2CPP_VM_RAISE_COM_EXCEPTION(hresult, defaultToCOMException) il2cpp::vm::Exception::Raise(hresult, defaultToCOMException)
    #define IL2CPP_VM_RAISE_IF_FAILED(hresult, defaultToCOMException) il2cpp::vm::Exception::RaiseIfFailed(hresult, defaultToCOMException)
    #define IL2CPP_VM_STRING_EMPTY() il2cpp::vm::String::Empty()
    #define IL2CPP_VM_STRING_NEW_UTF16(value, length) il2cpp::vm::String::NewUtf16(value, length)
    #define IL2CPP_VM_STRING_NEW_LEN(value, length) il2cpp::vm::String::NewLen(value, length)
    #define IL2CPP_VM_NOT_SUPPORTED(func, reason) NOT_SUPPORTED_IL2CPP(func, reason)
    #define IL2CPP_VM_METHOD_METADATA_FROM_INDEX(isGeneric, methodIndex) il2cpp::vm::MetadataCache::GetMethodInfoFromMethodDefinitionIndex (methodIndex)
    #define IL2CPP_VM_SHUTDOWN() il2cpp_shutdown()
    #define IL2CPP_VM_GET_CREATE_CCW_EXCEPTION(ex) vm::CCW::GetOrCreate(reinterpret_cast<Il2CppObject*>(ex), Il2CppIUnknown::IID)
typedef Il2CppString VmString;
typedef MethodInfo VmMethod;
#endif
