#pragma once

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

/* first setup platform defines*/
#if defined(SN_TARGET_PSP2)
    #define IL2CPP_TARGET_PSP2 1
    #define _UNICODE 1
    #define UNICODE 1
    #include "il2cpp-config-psp2.h"
#elif defined(SN_TARGET_ORBIS)
    #define IL2CPP_TARGET_PS4 1
    #define _UNICODE 1
    #define UNICODE 1
#elif defined(_MSC_VER)
    #define IL2CPP_TARGET_WINDOWS 1
    #if defined(_XBOX_ONE)
        #define IL2CPP_TARGET_XBOXONE 1
    #elif defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)
        #define IL2CPP_TARGET_WINRT 1
    #else
        #define IL2CPP_TARGET_WINDOWS_DESKTOP 1
    #endif
    #define _UNICODE 1
    #define UNICODE 1
    #define STRICT 1
#elif defined(__APPLE__)
    #define IL2CPP_TARGET_DARWIN 1
    #include "TargetConditionals.h"
    #if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR || TARGET_TVOS_SIMULATOR
        #define IL2CPP_TARGET_IOS 1
    #endif
#elif defined(__ANDROID__)
    #define IL2CPP_TARGET_ANDROID 1
#elif defined(EMSCRIPTEN)
    #define IL2CPP_TARGET_JAVASCRIPT 1
#elif defined(TIZEN)
    #define IL2CPP_TARGET_TIZEN 1
#elif defined(__linux__)
    #define IL2CPP_TARGET_LINUX 1
#elif defined(NN_PLATFORM_CTR)
    #define IL2CPP_TARGET_N3DS 1
#elif defined(NN_BUILD_TARGET_PLATFORM_NX)
    #define IL2CPP_TARGET_SWITCH 1
#else
    #error please define your target platform
#endif

#ifndef IL2CPP_TARGET_WINDOWS
#define IL2CPP_TARGET_WINDOWS 0
#endif

#ifndef IL2CPP_TARGET_WINDOWS_DESKTOP
#define IL2CPP_TARGET_WINDOWS_DESKTOP 0
#endif

#ifndef IL2CPP_TARGET_WINRT
#define IL2CPP_TARGET_WINRT 0
#endif

#ifndef IL2CPP_TARGET_XBOXONE
#define IL2CPP_TARGET_XBOXONE 0
#endif

#ifndef IL2CPP_TARGET_DARWIN
#define IL2CPP_TARGET_DARWIN 0
#endif

#ifndef IL2CPP_TARGET_IOS
#define IL2CPP_TARGET_IOS 0
#endif

#ifndef IL2CPP_TARGET_ANDROID
#define IL2CPP_TARGET_ANDROID 0
#endif

#ifndef IL2CPP_TARGET_JAVASCRIPT
#define IL2CPP_TARGET_JAVASCRIPT 0
#endif

#ifndef IL2CPP_TARGET_TIZEN
#define IL2CPP_TARGET_TIZEN 0
#endif

#ifndef IL2CPP_TARGET_LINUX
#define IL2CPP_TARGET_LINUX 0
#endif

#ifndef IL2CPP_TARGET_N3DS
#define IL2CPP_TARGET_N3DS 0
#endif

#ifndef IL2CPP_TARGET_PS4
#define IL2CPP_TARGET_PS4 0
#endif

#ifndef IL2CPP_TARGET_PSP2
#define IL2CPP_TARGET_PSP2 0
#endif

#ifndef IL2CPP_TARGET_SWITCH
#define IL2CPP_TARGET_SWITCH 0
#endif

#define IL2CPP_TARGET_POSIX (IL2CPP_TARGET_DARWIN || IL2CPP_TARGET_JAVASCRIPT || IL2CPP_TARGET_LINUX || IL2CPP_TARGET_ANDROID || IL2CPP_TARGET_PS4 || IL2CPP_TARGET_PSP2 || IL2CPP_TARGET_TIZEN)
#define IL2CPP_COMPILER_MSVC (IL2CPP_TARGET_WINDOWS || IL2CPP_TARGET_XBOXONE)

#ifndef IL2CPP_EXCEPTION_DISABLED
#define IL2CPP_EXCEPTION_DISABLED 0
#endif

// If the platform loads il2cpp as a dynamic library but does not have dlsym (or equivalent) then
// define IL2CPP_API_DYNAMIC_NO_DLSYM = 1 to add support for api function registration and symbol
// lookup APIs, see il2cpp-api.cpp
#ifndef IL2CPP_API_DYNAMIC_NO_DLSYM
#define IL2CPP_API_DYNAMIC_NO_DLSYM 0
#endif

#ifdef _MSC_VER
# include <malloc.h>
# define IL2CPP_EXPORT __declspec(dllexport)
# define IL2CPP_IMPORT __declspec(dllimport)
#elif IL2CPP_TARGET_PSP2 || IL2CPP_TARGET_PS4
# define IL2CPP_EXPORT __declspec(dllexport)
# define IL2CPP_IMPORT __declspec(dllimport)
#else
# define IL2CPP_EXPORT __attribute__ ((visibility ("default")))
# define IL2CPP_IMPORT
#endif

#ifdef LIBIL2CPP_EXPORT_CODEGEN_API
# define LIBIL2CPP_CODEGEN_API IL2CPP_EXPORT
#elif LIBIL2CPP_IMPORT_CODEGEN_API
# define LIBIL2CPP_CODEGEN_API IL2CPP_IMPORT
#else
# define LIBIL2CPP_CODEGEN_API
#endif

#if IL2CPP_TARGET_WINDOWS || IL2CPP_TARGET_XBOXONE || IL2CPP_TARGET_WINRT
#include <crtdbg.h>
#define IL2CPP_ASSERT(expr) \
    _ASSERTE(expr)
#else
#define IL2CPP_ASSERT(expr) \
    assert(expr)
#endif

#if defined(__ARMCC_VERSION)
    #include <assert.h>
    #include <wchar.h>
    #include <ctype.h>
    #define INTPTR_MAX 2147483647
#endif

#if IL2CPP_TARGET_DARWIN
    #define IL2CPP_METHOD_ATTR
// the following gives more accurate managed stack traces, but may cause linker errors on ARMv7 builds
// #define IL2CPP_METHOD_ATTR __attribute__((section ("__TEXT,__managed,regular,pure_instructions")))
#else
    #define IL2CPP_METHOD_ATTR
#endif

#if defined(_MSC_VER)
    #if defined(_M_X64)
        #define IL2CPP_SIZEOF_VOID_P 8
    #elif defined(_M_IX86) || defined(_M_ARM)
        #define IL2CPP_SIZEOF_VOID_P 4
    #else
        #error invalid windows architecture
    #endif
#elif defined(__GNUC__) || defined(__SNC__)
    #if defined(__x86_64__)
        #define IL2CPP_SIZEOF_VOID_P 8
    #elif defined(__i386__)
        #define IL2CPP_SIZEOF_VOID_P 4
    #elif defined(EMSCRIPTEN)
        #define IL2CPP_SIZEOF_VOID_P 4
    #elif defined(__arm__)
        #define IL2CPP_SIZEOF_VOID_P 4
    #elif defined(__arm64__)
        #define IL2CPP_SIZEOF_VOID_P 8
    #else
        #error invalid windows architecture
    #endif
#else
    #error please define your target architecture size
#endif

#if defined(_MSC_VER)
#define IL2CPP_CXX_ABI_MSVC 1
#else
#define IL2CPP_CXX_ABI_MSVC 0
#endif

#if IL2CPP_COMPILER_MSVC
#define STDCALL __stdcall
#define CDECL __cdecl
#else
#define STDCALL
#define CDECL
#endif

#if IL2CPP_COMPILER_MSVC || defined(__ARMCC_VERSION)
#define NORETURN __declspec(noreturn)
#else
#define NORETURN
#endif

#if IL2CPP_TARGET_IOS || IL2CPP_TARGET_ANDROID || IL2CPP_TARGET_DARWIN
#define REAL_NORETURN __attribute__ ((noreturn))
#else
#define REAL_NORETURN NORETURN
#endif

#if IL2CPP_COMPILER_MSVC || defined(__ARMCC_VERSION)
#define IL2CPP_NO_INLINE __declspec(noinline)
#else
#define IL2CPP_NO_INLINE __attribute__ ((noinline))
#endif

#if IL2CPP_COMPILER_MSVC
#define NOVTABLE __declspec(novtable)
#else
#define NOVTABLE
#endif

#define IL2CPP_ENABLE_MONO_BUG_EMULATION 1

// We currently use ALIGN_TYPE just for types decorated with IL2CPPStructAlignment, as it's needed for WebGL to properly align UnityEngine.Color.
// On MSVC, it causes build issues on x86 since you cannot pass aligned type by value as an argument to a function:
// error C2719: 'value': formal parameter with requested alignment of 16 won't be aligned
// Since this isn't actually needed for Windows, and it's not a standard .NET feature but just IL2CPP extension, let's just turn it off on Windows
#if defined(__GNUC__) || defined(__SNC__) || defined(__clang__)
    #define ALIGN_OF(T) __alignof__(T)
    #define ALIGN_TYPE(val) __attribute__((aligned(val)))
    #define ALIGN_FIELD(val) ALIGN_TYPE(val)
    #define FORCE_INLINE inline __attribute__ ((always_inline))
#elif defined(_MSC_VER)
    #define ALIGN_OF(T) __alignof(T)
    #define ALIGN_TYPE(val)
    #define ALIGN_FIELD(val) __declspec(align(val))
    #define FORCE_INLINE __forceinline
#else
    #define ALIGN_TYPE(size)
    #define ALIGN_FIELD(size)
    #define FORCE_INLINE inline
#endif

#define IL2CPP_PAGE_SIZE 4096

/* Trigger assert if 'ptr' is not aligned to 'alignment'. */
#define ASSERT_ALIGNMENT(ptr, alignment) \
    IL2CPP_ASSERT((((ptrdiff_t) ptr) & (alignment - 1)) == 0 && "Unaligned pointer!")

// 64-bit types are aligned to 8 bytes on 64-bit platforms and always on Windows
#define IL2CPP_ENABLE_INTERLOCKED_64_REQUIRED_ALIGNMENT ((IL2CPP_SIZEOF_VOID_P == 8) || (IL2CPP_TARGET_WINDOWS))

/* Debugging */
#ifndef IL2CPP_DEBUG
#define IL2CPP_DEBUG 0
#endif

#ifndef IL2CPP_DEVELOPMENT
#define IL2CPP_DEVELOPMENT 0
#endif

/* Threading */
#define IL2CPP_SUPPORT_THREADS !IL2CPP_TARGET_JAVASCRIPT

#define IL2CPP_USE_STD_THREAD 0

#define IL2CPP_THREADS_STD IL2CPP_USE_STD_THREAD
#define IL2CPP_THREADS_PTHREAD (!IL2CPP_THREADS_STD && IL2CPP_TARGET_POSIX)
#define IL2CPP_THREADS_WIN32 (!IL2CPP_THREADS_STD && IL2CPP_TARGET_WINDOWS)
#define IL2CPP_THREADS_N3DS (!IL2CPP_THREADS_STD && IL2CPP_TARGET_N3DS)
#define IL2CPP_THREADS_PS4 (!IL2CPP_THREADS_STD && IL2CPP_TARGET_PS4)
#define IL2CPP_THREADS_PSP2 (!IL2CPP_THREADS_STD && IL2CPP_TARGET_PSP2)
#define IL2CPP_THREADS_SWITCH (!IL2CPP_THREADS_STD && IL2CPP_TARGET_SWITCH)

#define IL2CPP_THREADS_ALL_ACCESS (!IL2CPP_THREADS_STD && IL2CPP_TARGET_XBOXONE)

#if (IL2CPP_SUPPORT_THREADS && (!IL2CPP_THREADS_STD && !IL2CPP_THREADS_PTHREAD && !IL2CPP_THREADS_WIN32 && !IL2CPP_THREADS_XBOXONE && !IL2CPP_THREADS_N3DS && !IL2CPP_THREADS_PS4 && !IL2CPP_THREADS_PSP2 && !IL2CPP_THREADS_SWITCH))
#error "No thread implementation defined"
#endif

/* Platform support to cleanup attached threads even when native threads are not exited cleanly */
#define IL2CPP_HAS_NATIVE_THREAD_CLEANUP (IL2CPP_THREADS_PTHREAD)

#define IL2CPP_THREAD_IMPL_HAS_COM_APARTMENTS IL2CPP_TARGET_WINDOWS

#if !defined(IL2CPP_ENABLE_PLATFORM_THREAD_STACKSIZE) && IL2CPP_TARGET_IOS
#define IL2CPP_ENABLE_PLATFORM_THREAD_STACKSIZE 1
#endif

#define IL2CPP_ENABLE_STACKTRACES 1
/* Platforms which use OS specific implementation to extract stracktrace */
#if !defined(IL2CPP_ENABLE_NATIVE_STACKTRACES)
#define IL2CPP_ENABLE_NATIVE_STACKTRACES (IL2CPP_TARGET_WINDOWS || IL2CPP_TARGET_LINUX || IL2CPP_TARGET_DARWIN || IL2CPP_TARGET_IOS || IL2CPP_TARGET_TIZEN || IL2CPP_TARGET_ANDROID)
#endif

/* Platforms which use stacktrace sentries */
#define IL2CPP_ENABLE_STACKTRACE_SENTRIES (IL2CPP_TARGET_JAVASCRIPT || IL2CPP_TARGET_N3DS || IL2CPP_TARGET_SWITCH)

#if (IL2CPP_ENABLE_STACKTRACES && !IL2CPP_ENABLE_NATIVE_STACKTRACES && !IL2CPP_ENABLE_STACKTRACE_SENTRIES)
#error "If stacktraces are supported, then either native stack traces must be supported, or usage of stacktrace sentries must be enabled!"
#endif

#if (IL2CPP_ENABLE_NATIVE_STACKTRACES + IL2CPP_ENABLE_STACKTRACE_SENTRIES) > 1
#error "Only one type of stacktraces are allowed"
#endif

#define IL2CPP_CAN_USE_MULTIPLE_SYMBOL_MAPS IL2CPP_TARGET_IOS

/* Profiler */
#ifndef IL2CPP_ENABLE_PROFILER
#define IL2CPP_ENABLE_PROFILER 1
#endif

/* GC defines*/
#define IL2CPP_GC_BOEHM 1
#define IL2CPP_GC_NULL !IL2CPP_GC_BOEHM
#define IL2CPP_ENABLE_DEFERRED_GC   IL2CPP_TARGET_JAVASCRIPT

/* we always need to NULL pointer free memory with our current allocators */
#define NEED_TO_ZERO_PTRFREE 1
#define IL2CPP_HAS_GC_DESCRIPTORS 1

#if defined(_MSC_VER)
    #define IL2CPP_ZERO_LEN_ARRAY 0
#else
    #define IL2CPP_ZERO_LEN_ARRAY 0
#endif

#define IL2CPP_HAS_CXX_CONSTEXPR (__has_feature (cxx_constexpr))

/* clang specific __has_builtin check */
#ifndef __has_builtin
    #define __has_builtin(x) 0 // Compatibility with non-clang compilers.
#endif

#if _MSC_VER
#define IL2CPP_UNREACHABLE __assume(0)
#elif __has_builtin(__builtin_unreachable)
#define IL2CPP_UNREACHABLE __builtin_unreachable()
#else
#define IL2CPP_UNREACHABLE
#endif

typedef uint32_t Il2CppMethodSlot;

/* Debug macros */
#define STRINGIZE(L)          #L
#define MAKE_STRING(M, L)     M(L)
#define $Line                   MAKE_STRING( STRINGIZE, __LINE__ )
#define FIXME                   __FILE__ "(" $Line ") : FIXME: "
#define ICALLMESSAGE(name)      __FILE__ "(" $Line ") : FIXME: Missing internal call implementation: " name
#define RUNTIMEMESSAGE(name)    __FILE__ "(" $Line ") : FIXME: Missing runtime implementation: " name
#define NOTSUPPORTEDICALLMESSAGE(target, name, reason)  __FILE__ "(" $Line ") : Unsupported internal call for " target ":" name " - " reason

// Keeping this for future usage if needed.
//#if defined(_MSC_VER)
//  #define PRAGMA_MESSAGE(value) __pragma(message(value))
//#else
//  #define PRAGMA_MESSAGE(value) _Pragma(STRINGIZE(value))
//#endif
#define PRAGMA_MESSAGE(value)

#if !defined(EMSCRIPTEN)

#define NOT_IMPLEMENTED_ICALL(func) \
    PRAGMA_MESSAGE(ICALLMESSAGE(#func)) \
    IL2CPP_ASSERT(0 && #func)
#define NOT_IMPLEMENTED_ICALL_NO_ASSERT(func, reason) \
    PRAGMA_MESSAGE(ICALLMESSAGE(#func))

#define NOT_IMPLEMENTED(func) \
    PRAGMA_MESSAGE(RUNTIMEMESSAGE(#func)) \
    IL2CPP_ASSERT(0 && #func)
#define NOT_IMPLEMENTED_NO_ASSERT(func, reason) \
    PRAGMA_MESSAGE(RUNTIMEMESSAGE(#func))

#else

// emscripten's assert will throw an exception in js.
// For now, we don't want that, so just printf and move on.
    #define NOT_IMPLEMENTED_ICALL(func) \
    PRAGMA_MESSAGE(message(ICALLMESSAGE(#func))) \
    printf("Not implemented icall: %s\n", #func);
#define NOT_IMPLEMENTED_ICALL_NO_ASSERT(func, reason) \
    PRAGMA_MESSAGE(message(ICALLMESSAGE(#func)))

#define NOT_IMPLEMENTED(func) \
    PRAGMA_MESSAGE(message(RUNTIMEMESSAGE(#func))) \
    printf("Not implemented: %s\n", #func);
#define NOT_IMPLEMENTED_NO_ASSERT(func, reason) \
    PRAGMA_MESSAGE(message(RUNTIMEMESSAGE(#func)))

#endif

#define NOT_SUPPORTED_IL2CPP(func, reason) \
    il2cpp::vm::Exception::Raise (il2cpp::vm::Exception::GetNotSupportedException ( NOTSUPPORTEDICALLMESSAGE ("IL2CPP", #func, #reason) ))

#define NOT_SUPPORTED_SRE(func) \
    il2cpp::vm::Exception::Raise (il2cpp::vm::Exception::GetNotSupportedException ( NOTSUPPORTEDICALLMESSAGE ("IL2CPP", #func, "System.Reflection.Emit is not supported.") ))

#define NOT_SUPPORTED_REMOTING(func) \
    il2cpp::vm::Exception::Raise (il2cpp::vm::Exception::GetNotSupportedException ( NOTSUPPORTEDICALLMESSAGE ("IL2CPP", #func, "System.Runtime.Remoting is not supported.") ))

#if IL2CPP_TARGET_JAVASCRIPT
#define NOT_SUPPORTED_WEBGL(func, reason) \
    il2cpp::vm::Exception::Raise (il2cpp::vm::Exception::GetNotSupportedException ( NOTSUPPORTEDICALLMESSAGE ("WebGL", #func, #reason) ))
#else
#define NOT_SUPPORTED_WEBGL(func, reason)
#endif

#if IL2CPP_COMPILER_MSVC
    #define IL2CPP_DIR_SEPARATOR '\\'   /* backslash */
#else
    #define IL2CPP_DIR_SEPARATOR '/'    /* forward slash */
#endif

#ifndef IL2CPP_DEBUGGER_ENABLED
    #define IL2CPP_DEBUGGER_ENABLED 0
#endif

#ifndef IL2CPP_DEBUGGER_LOG
    #define IL2CPP_DEBUGGER_LOG 0
#endif

#ifndef IL2CPP_DISABLE_FULL_MESSAGES
    #define IL2CPP_DISABLE_FULL_MESSAGES    1
#endif

#if IL2CPP_COMPILER_MSVC
    #define IL2CPP_USE_GENERIC_SOCKET_IMPL  0
#else
    #define IL2CPP_USE_GENERIC_SOCKET_IMPL  (!IL2CPP_TARGET_POSIX || IL2CPP_TARGET_JAVASCRIPT) &&  (!IL2CPP_TARGET_SWITCH)
#endif

/* set by platforms that require special handling of SIGPIPE signalling during socket sends */
#ifndef IL2CPP_USE_SEND_NOSIGNAL
    #define IL2CPP_USE_SEND_NOSIGNAL 0
#endif

#define IL2CPP_USE_GENERIC_ENVIRONMENT  (!IL2CPP_TARGET_WINDOWS && !IL2CPP_TARGET_POSIX)

#define IL2CPP_USE_GENERIC_COM  (!IL2CPP_TARGET_WINDOWS)
#define IL2CPP_USE_GENERIC_COM_SAFEARRAYS   (!IL2CPP_TARGET_WINDOWS || IL2CPP_TARGET_XBOXONE)
#define IL2CPP_USE_GENERIC_WINDOWSRUNTIME (!IL2CPP_TARGET_WINDOWS || RUNTIME_MONO || RUNTIME_NONE)

#ifndef IL2CPP_USE_GENERIC_MEMORY_MAPPED_FILE
#define IL2CPP_USE_GENERIC_MEMORY_MAPPED_FILE (IL2CPP_TARGET_XBOXONE || (!IL2CPP_TARGET_WINDOWS && !IL2CPP_TARGET_POSIX))
#endif

#define IL2CPP_SIZEOF_STRUCT_WITH_NO_INSTANCE_FIELDS 1
#define IL2CPP_VALIDATE_FIELD_LAYOUT 0

#if IL2CPP_COMPILER_MSVC    // Use stub "return false" implementation where it's not implemented
#define IL2CPP_ISDEBUGGERPRESENT_IMPLEMENTED 1
#else
#define IL2CPP_ISDEBUGGERPRESENT_IMPLEMENTED 0
#endif


#ifndef IL2CPP_USE_POSIX_COND_TIMEDWAIT_REL
#define IL2CPP_USE_POSIX_COND_TIMEDWAIT_REL ( IL2CPP_TARGET_DARWIN || IL2CPP_TARGET_ANDROID || IL2CPP_TARGET_PSP2 )
#endif

template<bool value>
struct Il2CppStaticAssertHelper;

template<>
struct Il2CppStaticAssertHelper<true>
{
};

#define Assert(x) do { (void)(x); IL2CPP_ASSERT(x); } while (false)
#define Il2CppStaticAssert(...) do { Il2CppStaticAssertHelper<(__VA_ARGS__)>(); } while (false)

const int32_t kIl2CppInt32Min = INT32_MIN;
const int32_t kIl2CppInt32Max = INT32_MAX;
const uint32_t kIl2CppUInt32Max = UINT32_MAX;
const int64_t kIl2CppInt64Min = INT64_MIN;
const int64_t kIl2CppInt64Max = INT64_MAX;
const uint64_t kIl2CppUInt64Max = UINT64_MAX;

#if IL2CPP_SIZEOF_VOID_P == 8
const intptr_t kIl2CppIntPtrMin = kIl2CppInt64Min;
const intptr_t kIl2CppIntPtrMax = kIl2CppInt64Max;
const uintptr_t kIl2CppUIntPtrMax = kIl2CppUInt64Max;
#else
const intptr_t kIl2CppIntPtrMin = kIl2CppInt32Min;
const intptr_t kIl2CppIntPtrMax = kIl2CppInt32Max;
const uintptr_t kIl2CppUIntPtrMax = kIl2CppUInt32Max;
#endif

const int ipv6AddressSize = 16;
#define IL2CPP_SUPPORT_IPV6 !IL2CPP_TARGET_PS4 && !IL2CPP_TARGET_SWITCH

// Android: "There is no support for locales in the C library" https://code.google.com/p/android/issues/detail?id=57313
// PS4/PS2: strtol_d doesn't exist
#define IL2CPP_SUPPORT_LOCALE_INDEPENDENT_PARSING (!IL2CPP_TARGET_ANDROID && !IL2CPP_TARGET_PS4 && !IL2CPP_TARGET_PSP2)

#define NO_UNUSED_WARNING(expr) (void)(expr)

typedef int32_t il2cpp_hresult_t;

// Sorted numerically!
#define IL2CPP_S_OK                          ((il2cpp_hresult_t)0)
#define IL2CPP_E_BOUNDS                      ((il2cpp_hresult_t)0x8000000B)
#define IL2CPP_E_CHANGED_STATE               ((il2cpp_hresult_t)0x8000000C)
#define IL2CPP_E_ILLEGAL_METHOD_CALL         ((il2cpp_hresult_t)0x8000000E)
#define IL2CPP_RO_E_CLOSED                   ((il2cpp_hresult_t)0x80000013)
#define IL2CPP_E_NOTIMPL                     ((il2cpp_hresult_t)0x80004001)
#define IL2CPP_E_NOINTERFACE                 ((il2cpp_hresult_t)0x80004002)
#define IL2CPP_E_POINTER                     ((il2cpp_hresult_t)0x80004003)
#define IL2CPP_E_ABORT                       ((il2cpp_hresult_t)0x80004004)
#define IL2CPP_E_FAIL                        ((il2cpp_hresult_t)0x80004005)
#define IL2CPP_E_UNEXPECTED                  ((il2cpp_hresult_t)0x8000FFFF)
#define IL2CPP_RPC_E_DISCONNECTED            ((il2cpp_hresult_t)0x80010108)
#define IL2CPP_RPC_E_WRONG_THREAD            ((il2cpp_hresult_t)0x8001010E)
#define IL2CPP_DISP_E_PARAMNOTFOUND          ((il2cpp_hresult_t)0x80020004)
#define IL2CPP_REGDB_E_CLASSNOTREG           ((il2cpp_hresult_t)0x80040154)
#define IL2CPP_E_ACCESS_DENIED               ((il2cpp_hresult_t)0x80070005)
#define IL2CPP_E_OUTOFMEMORY                 ((il2cpp_hresult_t)0x8007000E)
#define IL2CPP_E_INVALIDARG                  ((il2cpp_hresult_t)0x80070057)
#define IL2CPP_COR_E_EXCEPTION               ((il2cpp_hresult_t)0x80131500)
#define IL2CPP_COR_E_INVALIDOPERATION        ((il2cpp_hresult_t)0x80131509)
#define IL2CPP_COR_E_PLATFORMNOTSUPPORTED    ((il2cpp_hresult_t)0x80131539)
#define IL2CPP_COR_E_OPERATIONCANCELED       ((il2cpp_hresult_t)0x8013153B)
#define IL2CPP_COR_E_OBJECTDISPOSED          ((il2cpp_hresult_t)0x80131622)

#define IL2CPP_HR_SUCCEEDED(hr) (((il2cpp_hresult_t)(hr)) >= 0)
#define IL2CPP_HR_FAILED(hr) (((il2cpp_hresult_t)(hr)) < 0)

#include "il2cpp-api-types.h"

#define IL2CPP_LITTLE_ENDIAN 1
#define IL2CPP_BIG_ENDIAN 2
#define IL2CPP_BYTE_ORDER IL2CPP_LITTLE_ENDIAN

#if (defined(_MSC_VER) && _MSC_VER > 1600) || (__has_feature(cxx_override_control))
#define IL2CPP_OVERRIDE override
#define IL2CPP_FINAL final
#else
#define IL2CPP_OVERRIDE
#define IL2CPP_FINAL
#endif

#if (__has_feature(cxx_deleted_functions) || (defined(_MSC_VER) && _MSC_VER >= 1800))
#define IL2CPP_HAS_DELETED_FUNCTIONS 1
#else
#define IL2CPP_HAS_DELETED_FUNCTIONS 0
#endif

#if IL2CPP_TARGET_WINDOWS
const Il2CppChar kIl2CppNewLine[] = { '\r', '\n', '\0' };
#else
const Il2CppChar kIl2CppNewLine[] = { '\n', '\0' };
#endif


#if IL2CPP_TARGET_ANDROID && defined(__i386__)
// On Android with x86, function pointers are not aligned, so we
// need to use all of the bits when comparing them. Hence we mask
// nothing.
#define IL2CPP_POINTER_SPARE_BITS 0
#else
// On ARMv7 with Thumb instructions the lowest bit is always set.
// With Thumb2 the second-to-lowest bit is also set. Mask both of
// them off so that we can do a comparison properly based on the data
// from the linker map file. On other architectures this operation should
// not matter, as we assume these two bits are always zero because the pointer
// will be aligned.
#define IL2CPP_POINTER_SPARE_BITS 3
#endif

#define MAXIMUM_NESTED_GENERICS_EXCEPTION_MESSAGE "IL2CPP encountered a managed type which it cannot convert ahead-of-time. The type uses generic or array types which are nested beyond the maximum depth which can be converted."
#if IL2CPP_COMPILER_MSVC
#define IL2CPP_ATTRIBUTE_WEAK
#else
#define IL2CPP_ATTRIBUTE_WEAK __attribute__((weak))
#endif
