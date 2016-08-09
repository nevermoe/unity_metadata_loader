#pragma once

#include <stdint.h>
#include "il2cpp-config.h"

struct Il2CppObject;

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{

class LIBIL2CPP_CODEGEN_API GC
{
public:
	static int get_MaxGeneration();
	static int32_t CollectionCount (int32_t generation);
	static int32_t GetGeneration (Il2CppObject* obj);
	static int64_t GetTotalMemory (bool forceFullCollection);
	static void InternalCollect(int generation);
	static void KeepAlive (Il2CppObject* obj);
	static void ReRegisterForFinalize (Il2CppObject* obj);
	static void SuppressFinalize(Il2CppObject *);
	static void RecordPressure (int64_t bytesAllocated);
	static void WaitForPendingFinalizers();
};

} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
