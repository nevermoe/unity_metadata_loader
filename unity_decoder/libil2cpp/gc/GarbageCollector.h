#pragma once

struct Il2CppObject;
struct Il2CppThread;

namespace il2cpp
{
namespace gc
{

class GarbageCollector
{
public:
	static void Collect (int maxGeneration);
	static int32_t CollectALittle ();
	static int32_t GetCollectionCount (int32_t generation);
	static int64_t GetUsedHeapSize ();

public:
	// internal

	// functions implemented in a GC agnostic manner
	static void InitializeFinalizer ();
	static bool IsFinalizerThread (Il2CppThread* thread);
	static int32_t GetGeneration (void* addr);
	static void Uninitialize ();
	static void NotifyFinalizers ();
	static void RunFinalizer(void *obj, void *data);
	static void RegisterFinalizer(Il2CppObject* obj);
	static void SuppressFinalizer(Il2CppObject* obj);
	static void WaitForPendingFinalizers();
	static int32_t GetMaxGeneration();
	static void AddMemoryPressure(int64_t value);

	// functions implemented in a GC specific manner
	static void Initialize();
	static void Enable ();
	static void Disable ();

	static void RegisterFinalizerWithCallback(Il2CppObject* obj, void(*callback)(void *, void *));

	static int64_t GetAllocatedHeapSize ();

	static void* MakeDescriptorForObject (size_t *bitmap, int numbits);
	static void* MakeDescriptorForString ();
	static void* MakeDescriptorForArray ();

	static void* AllocateFixed (size_t size, void *descr);
	static void FreeFixed (void* addr);

	static bool RegisterThread (void *baseptr);
	static bool UnregisterThread ();

	static bool HasPendingFinalizers ();
	static int32_t InvokeFinalizers ();

	static void AddWeakLink (void **link_addr, Il2CppObject *obj, bool track);
	static void RemoveWeakLink (void **link_addr);
	static Il2CppObject *GetWeakLink (void **link_addr);

	/* Used by liveness code */
	static void StopWorld ();
	static void StartWorld ();

	typedef void (*HeapSectionCallback) (void* user_data, void* start, void* end);
	static void ForEachHeapSection (void* user_data, HeapSectionCallback callback);
	static size_t GetSectionCount ();

	typedef void* (*GCCallWithAllocLockCallback)(void* user_data);
	static void* CallWithAllocLockHeld (GCCallWithAllocLockCallback callback, void* user_data);
};

} /* namespace vm */
} /* namespace il2cpp */
