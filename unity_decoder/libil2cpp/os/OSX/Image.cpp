#include "il2cpp-config.h"

#if IL2CPP_TARGET_DARWIN

#include <mach-o/dyld.h>
#include <vector>

namespace il2cpp
{
namespace os
{
namespace Image
{
    void* GetImageBase()
    {
        // OSX/iOS uses ASLR (address space layout randomization), so
        // find where the image is loaded. This is usually zero in the
        // debugger for an executable, but non-zero when a debugger is not
        // present.
        std::vector<char> path;
        uint32_t size = 0;
        int error = _NSGetExecutablePath(NULL, &size);
        IL2CPP_ASSERT(error == -1);
        if (error != -1)
            return NULL;

        path.reserve(size);
        error = _NSGetExecutablePath(&path[0], &size);
        IL2CPP_ASSERT(error == 0);
        if (error != 0)
            return NULL;

        // Assume that we are either in an executable, or a dynamic
        // library named UserAssembly.dylib (for the OSX standalone player)

        int userAssemblyImageIndex = -1;
        int executableImageIndex = -1;
        int numberOfImages = _dyld_image_count();
        for (uint32_t i = 0; i < numberOfImages; i++)
        {
            const char* imageName = _dyld_get_image_name(i);
            if (strstr(imageName, "UserAssembly.dylib") != NULL)
                userAssemblyImageIndex = i;
            else if (strcmp(imageName, &path[0]) == 0)
                executableImageIndex = i;
        }

        if (userAssemblyImageIndex != -1)
            return (void*)_dyld_get_image_vmaddr_slide(userAssemblyImageIndex);
        else if (executableImageIndex != -1)
            return (void*)_dyld_get_image_vmaddr_slide(executableImageIndex);

        return NULL;
    }
}
}
}

#endif
