#include "il2cpp-config.h"

#if IL2CPP_USE_GENERIC_MEMORY_MAPPED_FILE
#include "os/File.h"
#include "os/MemoryMappedFile.h"
#include "utils/Memory.h"

namespace il2cpp
{
namespace os
{
    void* MemoryMappedFile::Map(FileHandle* file, size_t length, size_t offset)
    {
        int error = 0;

        if (!length)
        {
            length = os::File::GetLength(file, &error);
            if (error != 0)
            {
                return NULL;
            }
        }

        void* buffer = IL2CPP_MALLOC(length);

        os::File::Seek(file, offset, 0, &error);
        if (error != 0)
        {
            IL2CPP_FREE(buffer);
            return NULL;
        }

        int bytesRead = File::Read(file, (char*)buffer, (int)length, &error);
        if (bytesRead != length || error != 0)
        {
            IL2CPP_FREE(buffer);
            return NULL;
        }

        return buffer;
    }

    void MemoryMappedFile::Unmap(void* address, size_t length)
    {
        IL2CPP_FREE(address);
    }
}
}
#endif
