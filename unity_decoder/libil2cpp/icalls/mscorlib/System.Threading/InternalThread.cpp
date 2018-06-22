#if NET_4_0
#include "il2cpp-config.h"
#include "InternalThread.h"
#include "utils/Memory.h"
#include "os/Mutex.h"
#include "os/Thread.h"

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
    void InternalThread::Thread_free_internal(Il2CppInternalThread* _this, Il2CppIntPtr handle)
    {
        delete _this->synch_cs;
        _this->synch_cs = NULL;

        IL2CPP_FREE(_this->name);

        delete static_cast<il2cpp::os::Thread*>(handle.m_value);
        handle.m_value = NULL;
    }
} // namespace Threading
} // namespace System
} // namespace mscorlib
} // namespace icalls
} // namespace il2cpp
#endif
