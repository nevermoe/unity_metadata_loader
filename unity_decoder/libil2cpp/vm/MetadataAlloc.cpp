#include "il2cpp-config.h"
#include "MetadataAlloc.h"
#include "class-internals.h"
#include "utils/MemoryPool.h"

using il2cpp::utils::MemoryPool;

namespace il2cpp
{
namespace vm
{
// we allocate these dynamically on runtime initialization
// because the pool uses standard allocators, and we want to give embedding
// client the chance to install their own allocator callbacks
    static MemoryPool* s_MetadataMemoryPool;
    static MemoryPool* s_GenericClassMemoryPool;
    static MemoryPool* s_GenericMethodMemoryPool;

// This initial size (256k/512k) allows us enough room to initialize metadata
// an empty Unity project and have a bit of room leftover.
    const size_t kInitialRegionSize = IL2CPP_SIZEOF_VOID_P * 64 * 1024;

    void MetadataAllocInitialize()
    {
        s_MetadataMemoryPool = new MemoryPool(kInitialRegionSize);
        // these can use the default smaller initial pool size
        s_GenericClassMemoryPool = new MemoryPool();
        s_GenericMethodMemoryPool = new MemoryPool();
    }

    void MetadataAllocCleanup()
    {
        delete s_MetadataMemoryPool;
        s_MetadataMemoryPool = NULL;
        delete s_GenericClassMemoryPool;
        s_GenericClassMemoryPool = NULL;
        delete s_GenericMethodMemoryPool;
        s_GenericMethodMemoryPool = NULL;
    }

    void* MetadataMalloc(size_t size)
    {
        return s_MetadataMemoryPool->Malloc(size);
    }

    void* MetadataCalloc(size_t count, size_t size)
    {
        return s_MetadataMemoryPool->Calloc(count, size);
    }

    Il2CppGenericClass* MetadataAllocGenericClass()
    {
        return (Il2CppGenericClass*)s_GenericClassMemoryPool->Calloc(1, sizeof(Il2CppGenericClass));
    }

    Il2CppGenericMethod* MetadataAllocGenericMethod()
    {
        return (Il2CppGenericMethod*)s_GenericMethodMemoryPool->Calloc(1, sizeof(Il2CppGenericMethod));
    }
}
}
