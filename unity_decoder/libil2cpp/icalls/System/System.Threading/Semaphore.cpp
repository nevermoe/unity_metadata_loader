#include "icalls/System/System.Threading/Semaphore.h"
#include "os/Semaphore.h"
#include "vm/Exception.h"

namespace il2cpp
{
namespace icalls
{
namespace System
{
namespace System
{
namespace Threading
{
#if !NET_4_0
    Il2CppIntPtr Semaphore::CreateSemaphore_internal(int32_t initialCount, int32_t maximumCount, Il2CppString* name, bool* created)
    {
        *created = true;
        os::Semaphore* semaphore = NULL;

        if (name == NULL)
            semaphore = new os::Semaphore(initialCount, maximumCount);
        else
            NOT_SUPPORTED_IL2CPP(Semaphore::CreateSemaphore_internal, "Named semaphores are not supported.");

        Il2CppIntPtr ret = { new os::SemaphoreHandle(semaphore) };
        return ret;
    }

    int32_t Semaphore::ReleaseSemaphore_internal(Il2CppIntPtr handlePtr, int32_t releaseCount, bool* fail)
    {
        os::SemaphoreHandle* handle = (os::SemaphoreHandle*)handlePtr.m_value;

        int32_t previousCount = 0;
        *fail = !handle->Get().Post(releaseCount, &previousCount);

        return previousCount;
    }

#else
    Il2CppIntPtr Semaphore::CreateSemaphore_internal40(int32_t initialCount, int32_t maximumCount, Il2CppString* name, int32_t* errorCode)
    {
        *errorCode = true;
        os::Semaphore* semaphore = NULL;

        if (name == NULL)
            semaphore = new os::Semaphore(initialCount, maximumCount);
        else
            NOT_SUPPORTED_IL2CPP(Semaphore::CreateSemaphore_internal, "Named semaphores are not supported.");

        Il2CppIntPtr ret = { new os::SemaphoreHandle(semaphore) };
        return ret;
    }

    bool Semaphore::ReleaseSemaphore_internal40(Il2CppIntPtr handlePtr, int32_t releaseCount, int32_t* previousCount)
    {
        os::SemaphoreHandle* handle = (os::SemaphoreHandle*)handlePtr.m_value;

        return handle->Get().Post(releaseCount, previousCount);
    }

#endif

    Il2CppIntPtr Semaphore::OpenSemaphore_internal(Il2CppString* name, int32_t rights, int32_t* error)
    {
        NOT_SUPPORTED_IL2CPP(Semaphore::OpenSemaphore_internal, "Named semaphores are not supported.");

        return Il2CppIntPtr::Zero;
    }
} /* namespace Threading */
} /* namespace System */
} /* namespace System */
} /* namespace icalls */
} /* namespace il2cpp */
