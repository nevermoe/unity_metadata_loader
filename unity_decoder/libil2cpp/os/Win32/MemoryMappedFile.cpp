#include "il2cpp-config.h"

#if !IL2CPP_USE_GENERIC_MEMORY_MAPPED_FILE && IL2CPP_TARGET_WINDOWS

#include <map>
#include "WindowsHelpers.h"
#include "os/MemoryMappedFile.h"
#include "os/Mutex.h"
#include <limits>

namespace il2cpp
{
namespace os
{
    static FastMutex s_Mutex;
    static std::map<void*, HANDLE> s_MappedAddressToMappedFileObject;

    void* MemoryMappedFile::Map(FileHandle* file, size_t length, size_t offset)
    {
        os::FastAutoLock lock(&s_Mutex);

        HANDLE mappedFile = CreateFileMapping((HANDLE)file, NULL, PAGE_READONLY, 0, 0, NULL);
        if (mappedFile == NULL)
            return NULL;

        IL2CPP_ASSERT(offset <= std::numeric_limits<DWORD>::max());
        IL2CPP_ASSERT(length <= std::numeric_limits<DWORD>::max());

        void* address = MapViewOfFile(mappedFile, FILE_MAP_READ, 0, static_cast<DWORD>(offset), static_cast<DWORD>(length));
        if (address == NULL)
        {
            DWORD error = GetLastError();

            CloseHandle(mappedFile);
            return NULL;
        }

        s_MappedAddressToMappedFileObject[address] = mappedFile;

        return address;
    }

    void MemoryMappedFile::Unmap(void* address, size_t length)
    {
        if (address != NULL)
        {
            os::FastAutoLock lock(&s_Mutex);

            BOOL error = UnmapViewOfFile(address);
            IL2CPP_ASSERT(error != 0);
            (void)error; // Avoid an unused variable warning

            std::map<void*, HANDLE>::iterator entry = s_MappedAddressToMappedFileObject.find(address);
            if (entry != s_MappedAddressToMappedFileObject.end())
            {
                error = CloseHandle(entry->second);
                IL2CPP_ASSERT(error != 0);
            }
        }
    }
}
}
#endif
