#include "il2cpp-config.h"
#include "icalls/mscorlib/System.Threading/Mutex.h"
#include "os/Mutex.h"
#include "vm/Exception.h"

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{
namespace Threading
{
    Il2CppIntPtr Mutex::CreateMutex_internal(bool initiallyOwned, Il2CppString* name, bool* created)
    {
        *created = true;
        il2cpp::os::Mutex* mutex = NULL;

        if (name == NULL)
        {
            mutex = new il2cpp::os::Mutex();
        }
        else
        {
            NOT_SUPPORTED_IL2CPP(Mutex::CreateMutex_internal, "Named mutexes are not supported");
        }

        if (initiallyOwned)
            mutex->Lock();

        Il2CppIntPtr ret = { new il2cpp::os::MutexHandle(mutex) };
        return ret;
    }

    bool Mutex::ReleaseMutex_internal(Il2CppIntPtr handle)
    {
        il2cpp::os::MutexHandle* mutex = (il2cpp::os::MutexHandle*)handle.m_value;
        mutex->Get()->Unlock();
        return true;
    }

    Il2CppIntPtr Mutex::OpenMutex_internal(Il2CppString* name, MutexRights rights, MonoIOError* error)
    {
        NOT_IMPLEMENTED_ICALL(Mutex::OpenMutex_internal);

        return Il2CppIntPtr();
    }
} /* namespace Threading */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
