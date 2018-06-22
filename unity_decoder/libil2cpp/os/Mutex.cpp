#include "il2cpp-config.h"

#include "os/Mutex.h"
#include "os/Atomic.h"
#if IL2CPP_THREADS_WIN32
#include "os/Win32/MutexImpl.h"
#elif IL2CPP_TARGET_PSP2
#include "os/PSP2/MutexImpl.h"
#elif IL2CPP_THREADS_PTHREAD
#include "os/Posix/MutexImpl.h"
#else
#include "os/MutexImpl.h"
#endif

namespace il2cpp
{
namespace os
{
    Mutex::Mutex(bool initiallyOwned)
        : m_Mutex(new MutexImpl())
    {
        if (initiallyOwned)
            Lock();
    }

    Mutex::~Mutex()
    {
        delete m_Mutex;
    }

    void Mutex::Lock(bool interruptible)
    {
        m_Mutex->Lock(interruptible);
    }

    bool Mutex::TryLock(uint32_t milliseconds, bool interruptible)
    {
        return m_Mutex->TryLock(milliseconds, interruptible);
    }

    void Mutex::Unlock()
    {
        m_Mutex->Unlock();
    }

    FastMutex::FastMutex()
        : m_Impl(new FastMutexImpl())
    {
    }

    FastMutex::~FastMutex()
    {
        delete m_Impl;
    }

    void FastMutex::Lock()
    {
        m_Impl->Lock();
    }

    void FastMutex::Unlock()
    {
        m_Impl->Unlock();
    }

    FastMutexImpl* FastMutex::GetImpl()
    {
        return m_Impl;
    }
}
}
