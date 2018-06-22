#pragma once

#if NET_4_0
#include "object-internals.h"

namespace il2cpp
{
namespace icalls
{
namespace System
{
namespace Core
{
namespace System
{
namespace IO
{
namespace MemoryMappedFiles
{
    class LIBIL2CPP_CODEGEN_API MemoryMapImpl
    {
    public:
        static bool Unmap(Il2CppIntPtr mmap_handle);
        static int32_t MapInternal(Il2CppIntPtr handle, int64_t offset, int64_t* size, int32_t access, Il2CppIntPtr* mmap_handle, Il2CppIntPtr* base_address);
        static Il2CppIntPtr OpenFileInternal(Il2CppString* path, int32_t mode, Il2CppString* mapName, int64_t* capacity, int32_t access, int32_t options, int32_t* error);
        static Il2CppIntPtr OpenHandleInternal(Il2CppIntPtr handle, Il2CppString* mapName, int64_t* capacity, int32_t access, int32_t options, int32_t* error);
        static void CloseMapping(Il2CppIntPtr handle);
        static void ConfigureHandleInheritability(Il2CppIntPtr handle, int32_t inheritability);
        static void Flush(Il2CppIntPtr file_handle);
    };
} // namespace MemoryMappedFiles
} // namespace IO
} // namespace System
} // namespace Core
} // namespace System
} // namespace icalls
} // namespace il2cpp
#endif
