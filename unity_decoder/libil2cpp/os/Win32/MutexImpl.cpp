#include "il2cpp-config.h"

#if IL2CPP_THREADS_WIN32

#include "MutexImpl.h"
#include "WindowsHelpers.h"
#include <cassert>

// Can't use critical sections as they don't allow for interruption by APCs.

namespace il2cpp
{
namespace os
{

MutexImpl::MutexImpl ()
{
	m_MutexHandle = ::CreateMutex (NULL, FALSE, NULL);
	assert (m_MutexHandle);
}

MutexImpl::~MutexImpl ()
{
	assert (m_MutexHandle);
	::CloseHandle (m_MutexHandle);
}

void MutexImpl::Lock (bool interruptible)
{
	TryLock (INFINITE, interruptible);
}

bool MutexImpl::TryLock (uint32_t milliseconds, bool interruptible)
{
	return (il2cpp::os::win::WaitForSingleObjectAndAccountForAPCs (m_MutexHandle, milliseconds, interruptible) == kWaitStatusSuccess);
}

void MutexImpl::Unlock ()
{
	ReleaseMutex (m_MutexHandle);
}

}
}

#endif 