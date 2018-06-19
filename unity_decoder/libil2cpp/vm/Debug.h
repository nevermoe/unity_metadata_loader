#pragma once
#include "il2cpp-config.h"
struct Il2CppThread;
struct Il2CppDebugDocument;
struct Il2CppDebugTypeInfo;
struct Il2CppDebugMethodInfo;
struct Il2CppDebugLocalsInfo;
struct Il2CppStackFrameInfo;

namespace il2cpp
{
namespace vm
{

class LIBIL2CPP_CODEGEN_API Debug
{
public:

	static const Il2CppDebugDocument* GetDocument (const Il2CppDebugTypeInfo* type);
	static const char *DocumentGetFilename (const Il2CppDebugDocument *document);
	static const char *DocumentGetDirectory (const Il2CppDebugDocument *document);
	static const Il2CppDebugDocument* GetDocument (const Il2CppDebugMethodInfo* info);
	static const int32_t* GetOffsetTable (const Il2CppDebugMethodInfo* info);
	static size_t GetCodeSize (const Il2CppDebugMethodInfo* info);
	static void UpdateFrameIlOffset(int32_t il_offset);
	static const Il2CppDebugLocalsInfo **GetLocalsInfo (const Il2CppDebugMethodInfo* info);
	static const Il2CppClass *GetType (const Il2CppDebugLocalsInfo *info);
	static const char *GetName (const Il2CppDebugLocalsInfo *info);
	static uint32_t GetStartOffset (const Il2CppDebugLocalsInfo *info);
	static uint32_t GetEndOffset (const Il2CppDebugLocalsInfo *info);
	static Il2CppObject *GetParamValue (const Il2CppStackFrameInfo *info, uint32_t position);
	static Il2CppObject *GetLocalValue (const Il2CppStackFrameInfo *info, uint32_t position);
	static void *GetBreakpointDataAt (const Il2CppDebugMethodInfo* info, int64_t uid, int32_t offset);
	static void SetBreakpointDataAt (const Il2CppDebugMethodInfo* info, uint64_t location, void *data);
	static void ClearBreakpointData (const Il2CppDebugMethodInfo* info);
	static void ClearBreakpointDataAt (const Il2CppDebugMethodInfo* info, uint64_t location);

public:
	//internal

private:

};

} /* namespace vm */
} /* namespace il2cpp */
