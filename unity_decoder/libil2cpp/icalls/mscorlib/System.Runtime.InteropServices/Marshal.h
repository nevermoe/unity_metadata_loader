#pragma once

#include <stdint.h>
#include "il2cpp-config.h"
#include "object-internals.h"

struct mscorlib_System_Guid;
struct mscorlib_System_Reflection_MemberInfo;

struct Il2CppObject;
struct Il2CppDelegate;
struct Il2CppReflectionType;
struct Il2CppString;

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{
namespace Runtime
{
namespace InteropServices
{

class LIBIL2CPP_CODEGEN_API Marshal
{
public:
	static int32_t GetLastWin32Error ();
	static int32_t AddRefInternal (Il2CppIntPtr pUnk);
	static Il2CppIntPtr AllocCoTaskMem (int32_t size);
	static Il2CppIntPtr AllocHGlobal (Il2CppIntPtr size);
	static void DestroyStructure (Il2CppIntPtr ptr, Il2CppReflectionType* structureType);
	static void FreeBSTR (Il2CppIntPtr ptr);
	static void FreeCoTaskMem (Il2CppIntPtr ptr);
	static void FreeHGlobal (Il2CppIntPtr hglobal);
	static Il2CppIntPtr GetCCW (Il2CppObject* o, Il2CppReflectionType * T);
	static int32_t GetComSlotForMethodInfoInternal (mscorlib_System_Reflection_MemberInfo * m);
	static Il2CppDelegate* GetDelegateForFunctionPointerInternal(Il2CppIntPtr ptr, Il2CppReflectionType* t);
	static Il2CppIntPtr GetFunctionPointerForDelegateInternal(Il2CppDelegate* d);
	static Il2CppIntPtr GetIDispatchForObjectInternal (Il2CppObject* o);
	static Il2CppIntPtr GetIUnknownForObjectInternal (Il2CppObject* o);
	static Il2CppObject* GetObjectForCCW (Il2CppIntPtr pUnk);
	static bool IsComObject (Il2CppObject* o);
	static Il2CppIntPtr OffsetOf (Il2CppReflectionType* t, Il2CppString* fieldName);
	static void Prelink (Il2CppReflectionMethod* m);
	static void PrelinkAll (Il2CppReflectionType* c);
	static Il2CppString* PtrToStringAnsi_mscorlib_System_String_mscorlib_System_IntPtr (Il2CppIntPtr ptr);
	static Il2CppString* PtrToStringAnsi_mscorlib_System_String_mscorlib_System_IntPtr_mscorlib_System_Int32 (Il2CppIntPtr ptr, int32_t len);
	static Il2CppString* PtrToStringBSTR (Il2CppIntPtr ptr);
	static Il2CppString* PtrToStringUni_mscorlib_System_String_mscorlib_System_IntPtr (Il2CppIntPtr ptr);
	static Il2CppString* PtrToStringUni_mscorlib_System_String_mscorlib_System_IntPtr_mscorlib_System_Int32 (Il2CppIntPtr ptr, int32_t len);
	static Il2CppObject* PtrToStructure (Il2CppIntPtr ptr, Il2CppReflectionType * structureType);
	static void PtrToStructureObject (Il2CppIntPtr ptr, Il2CppObject* structure);
	static int32_t QueryInterfaceInternal (Il2CppIntPtr pUnk, mscorlib_System_Guid * iid, Il2CppIntPtr* ppv);
	static Il2CppIntPtr ReAllocCoTaskMem (Il2CppIntPtr ptr, int32_t size);
	static Il2CppIntPtr ReAllocHGlobal (Il2CppIntPtr ptr, Il2CppIntPtr size);
	static uint8_t ReadByte (Il2CppIntPtr ptr, int32_t ofs);
	static int16_t ReadInt16 (Il2CppIntPtr ptr, int32_t ofs);
	static int32_t ReadInt32 (Il2CppIntPtr ptr, int32_t ofs);
	static int64_t ReadInt64 (Il2CppIntPtr ptr, int32_t ofs);
	static Il2CppIntPtr ReadIntPtr (Il2CppIntPtr ptr, int32_t ofs);
	static int32_t ReleaseComObjectInternal (Il2CppObject* co);
	static int32_t ReleaseInternal (Il2CppIntPtr pUnk);
	static int SizeOf (Il2CppReflectionType * rtype);
	static Il2CppIntPtr StringToBSTR (Il2CppString* s);
	static Il2CppIntPtr StringToHGlobalAnsi (Il2CppString* s);
	static Il2CppIntPtr StringToHGlobalUni (Il2CppString* s);
	static void StructureToPtr(Il2CppObject* structure, Il2CppIntPtr ptr, bool deleteOld);
	static Il2CppIntPtr UnsafeAddrOfPinnedArrayElement (Il2CppArray* arr, int32_t index);
	static void WriteByte (Il2CppIntPtr ptr, int32_t ofs, uint8_t val);
	static void WriteInt16 (Il2CppIntPtr ptr, int32_t ofs, int16_t val);
	static void WriteInt32 (Il2CppIntPtr ptr, int32_t ofs, int32_t val);
	static void WriteInt64 (Il2CppIntPtr ptr, int32_t ofs, int64_t val);
	static void copy_from_unmanaged (Il2CppIntPtr,int,Il2CppArray *,int);
	static void copy_to_unmanaged (Il2CppArray * source, int32_t startIndex, Il2CppIntPtr destination, int32_t length);
	static void WriteIntPtr (Il2CppIntPtr ptr, int32_t ofs, Il2CppIntPtr val);
};

} /* namespace InteropServices */
} /* namespace Runtime */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
