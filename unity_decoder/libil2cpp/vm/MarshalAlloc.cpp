#include "il2cpp-config.h"
#include "MarshalAlloc.h"
#include "os/MarshalAlloc.h"
#include <cassert>

namespace il2cpp
{
namespace vm
{

#if _DEBUG
static os::FastMutex s_Mutex; // Locking only necessary in a debug build.
static std::map<void*, size_t> s_Allocations;
#endif

void* MarshalAlloc::Allocate(size_t size)
{
	void* ptr = os::MarshalAlloc::Allocate(size);

#if _DEBUG
	os::FastAutoLock lock(&s_Mutex);
	s_Allocations[ptr] = size;
#endif

	return ptr;
}

void* MarshalAlloc::ReAlloc(void* ptr, size_t size)
{
	void* realloced = os::MarshalAlloc::ReAlloc(ptr, size);

#if _DEBUG
	os::FastAutoLock lock(&s_Mutex);
	if (ptr != NULL && ptr != realloced)
	{
		std::map<void*, size_t>::iterator found = s_Allocations.find(ptr);
		assert(found != s_Allocations.end() && "Invalid call to MarshalAlloc::ReAlloc. The pointer is not in the allocation list.");
		s_Allocations.erase(found);
	}
	s_Allocations[realloced] = size;
#endif

	return realloced;
}

void MarshalAlloc::Free(void* ptr)
{
#if _DEBUG
	os::FastAutoLock lock(&s_Mutex);
	std::map<void*, size_t>::iterator found = s_Allocations.find(ptr);
	if (found != s_Allocations.end())	// It might not be necessarily allocated by us, e.g. we might be freeing memory that's returned from native P/Invoke call
		s_Allocations.erase(found);
#endif

	os::MarshalAlloc::Free(ptr);
}

void* MarshalAlloc::AllocateHGlobal(size_t size)
{
	// should use Unity's memory management here!
	return malloc(size);
}

void* MarshalAlloc::ReAllocHGlobal(void* ptr, size_t size)
{
	// should use Unity's memory management here!
	return realloc(ptr, size);
}

void MarshalAlloc::FreeHGlobal (void* ptr)
{
	// should use Unity's memory management here!
	free(ptr);
}

#if _DEBUG

bool MarshalAlloc::HasUnfreedAllocations()
{
	os::FastAutoLock lock(&s_Mutex);
	return s_Allocations.size() != 0;
}

void MarshalAlloc::ClearAllTrackedAllocations()
{
	os::FastAutoLock lock(&s_Mutex);
	s_Allocations.clear();
}

#endif

} /* namespace vm */
} /* namespace il2cpp */
