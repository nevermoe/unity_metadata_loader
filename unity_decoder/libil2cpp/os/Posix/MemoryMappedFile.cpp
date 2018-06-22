#include "il2cpp-config.h"

#if !IL2CPP_USE_GENERIC_MEMORY_MAPPED_FILE && IL2CPP_TARGET_POSIX

#include <sys/mman.h>
#include <map>
#include "os/File.h"
#include "os/MemoryMappedFile.h"
#include "os/Mutex.h"
#include "FileHandle.h"

namespace il2cpp
{
namespace os
{
    static FastMutex s_Mutex;
    static std::map<void*, int64_t> s_MappedAddressToMappedLength;

    void* MemoryMappedFile::Map(FileHandle* file, size_t length, size_t offset)
    {
        os::FastAutoLock lock(&s_Mutex);

        if (length == 0)
        {
            int error = 0;
            length = File::GetLength(file, &error);
            if (error != 0)
                return NULL;
        }

        void* address = mmap(NULL, length, PROT_READ, MAP_FILE | MAP_PRIVATE, file->fd, offset);
        if ((intptr_t)address == -1)
            return NULL;

        s_MappedAddressToMappedLength[address] = length;

        return address;
    }

    void MemoryMappedFile::Unmap(void* address, size_t length)
    {
        os::FastAutoLock lock(&s_Mutex);

        if (length == 0)
        {
            std::map<void*, int64_t>::iterator entry = s_MappedAddressToMappedLength.find(address);
            if (entry != s_MappedAddressToMappedLength.end())
                length = entry->second;
        }

        int error = munmap(address, length);
        IL2CPP_ASSERT(error == 0);
        (void)error; // Avoid an unused variable warning
    }
}
}
#endif
