#pragma once

#include <stdint.h>
#include "il2cpp-config.h"

namespace il2cpp
{
namespace vm
{

#if IL2CPP_ENABLE_PROFILER

class LIBIL2CPP_CODEGEN_API Profiler
{
// exported
public:
	static void Install (Il2CppProfiler *prof, Il2CppProfileFunc shutdownCallback);
	static void SetEvents (Il2CppProfileFlags events);

	static void InstallEnterLeave (Il2CppProfileMethodFunc enter, Il2CppProfileMethodFunc fleave);
	static void InstallAllocation (Il2CppProfileAllocFunc callback);
	static void InstallGC (Il2CppProfileGCFunc callback, Il2CppProfileGCResizeFunc heap_resize_callback);

// internal
public:
	static void Allocation (Il2CppObject *obj, Il2CppClass *klass);
	static void MethodEnter (const MethodInfo *method);
	static void MethodExit (const MethodInfo *method);
	static void GCEvent (Il2CppGCEvent eventType);
	static void GCHeapResize (int64_t newSize);

	static Il2CppProfileFlags s_profilerEvents;

	static inline bool ProfileAllocations ()
	{
		return (s_profilerEvents & IL2CPP_PROFILE_ALLOCATIONS) != 0;
	}

private:
};

#endif

} /* namespace vm */
} /* namespace il2cpp */
