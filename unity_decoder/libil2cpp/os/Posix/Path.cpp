#include "il2cpp-config.h"

#if IL2CPP_TARGET_POSIX
#include "os/Environment.h"
#include "os/Path.h"
#include <string>

#if defined(__APPLE__)
#include "mach-o/dyld.h"
#elif IL2CPP_TARGET_LINUX || IL2CPP_TARGET_ANDROID
#include <linux/limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#endif

namespace il2cpp
{
namespace os
{
    std::string Path::GetExecutablePath()
    {
#if defined(__APPLE__)
        char path[1024];
        uint32_t size = sizeof(path);
        if (_NSGetExecutablePath(path, &size) == 0)
            return path;

        std::string result;
        result.resize(size + 1);
        _NSGetExecutablePath(&result[0], &size);
        return result;
#elif IL2CPP_TARGET_LINUX || IL2CPP_TARGET_ANDROID
        char path[PATH_MAX];
        char dest[PATH_MAX];
        struct stat info;
        pid_t pid = getpid();
        sprintf(path, "/proc/%d/exe", pid);
        if (readlink(path, dest, PATH_MAX) == -1)
            return std::string();
        return dest;
#else
        return std::string();
#endif
    }

    std::string Path::GetTempPath()
    {
        static const char* tmpdirs[] = { "TMPDIR", "TMP", "TEMP", NULL};

        for (size_t i = 0; tmpdirs[i] != NULL; ++i)
        {
            std::string tmpdir = Environment::GetEnvironmentVariable(tmpdirs[i]);

            if (!tmpdir.empty())
                return tmpdir;
        }

#if IL2CPP_TARGET_ANDROID
        return std::string("/data/local/tmp");
#else
        return std::string("/tmp");
#endif
    }
}
}

#endif
