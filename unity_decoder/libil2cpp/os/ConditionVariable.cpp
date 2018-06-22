#if NET_4_0
#include "il2cpp-config.h"

#include "os/Mutex.h"
#include "os/ConditionVariable.h"
#if IL2CPP_THREADS_WIN32
#include "os/Win32/ConditionVariableImpl.h"
#else
#include "os/Posix/ConditionVariableImpl.h"
#endif


namespace il2cpp
{
namespace os
{
    ConditionVariable::ConditionVariable()
        : m_ConditionVariable(new ConditionVariableImpl())
    {
    }

    ConditionVariable::~ConditionVariable()
    {
        delete m_ConditionVariable;
    }

    int ConditionVariable::Wait(FastMutex* lock)
    {
        return m_ConditionVariable->Wait(lock->GetImpl());
    }

    int ConditionVariable::TimedWait(FastMutex* lock, uint32_t timeout_ms)
    {
        return m_ConditionVariable->TimedWait(lock->GetImpl(), timeout_ms);
    }

    void ConditionVariable::Broadcast()
    {
        m_ConditionVariable->Broadcast();
    }

    void ConditionVariable::Signal()
    {
        m_ConditionVariable->Signal();
    }
}
}
#endif
