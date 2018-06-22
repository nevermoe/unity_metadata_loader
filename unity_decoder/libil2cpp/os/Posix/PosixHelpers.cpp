#include "il2cpp-config.h"

#if IL2CPP_TARGET_POSIX

#include <sys/errno.h>

#include "os/Posix/PosixHelpers.h"

namespace il2cpp
{
namespace os
{
namespace posix
{
    int Poll(pollfd* handles, int numHandles, int timeout)
    {
        int32_t ret = 0;
        time_t start = time(NULL);

        timeout = (timeout >= 0) ? (timeout / 1000) : -1;
        do
        {
            ret = poll(handles, numHandles, timeout);

            if (timeout > 0 && ret < 0)
            {
                const int32_t err = errno;
                const int32_t sec = time(NULL) - start;

                timeout -= sec * 1000;

                if (timeout < 0)
                    timeout = 0;

                errno = err;
            }
        }
        while (ret == -1 && errno == EINTR);

        return ret;
    }
}
}
}

#endif // IL2CPP_TARGET_POSIX
