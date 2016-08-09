#include "il2cpp-config.h"

#if IL2CPP_THREADS_WIN32

#include "ThreadLocalValueImpl.h"
#include <cassert>

namespace il2cpp
{
namespace os
{

ThreadLocalValueImpl::ThreadLocalValueImpl ()
{
	m_Index = TlsAlloc ();

	assert (m_Index != TLS_OUT_OF_INDEXES);
}

ThreadLocalValueImpl::~ThreadLocalValueImpl ()
{
	assert (TlsFree (m_Index));
}

ErrorCode ThreadLocalValueImpl::SetValue (void* value)
{
	if (TlsSetValue (m_Index, value) == FALSE)
		return static_cast<ErrorCode>(GetLastError());

	return kErrorCodeSuccess;
}
ErrorCode ThreadLocalValueImpl::GetValue (void** value)
{
	*value = TlsGetValue (m_Index);
	if (*value)
		return kErrorCodeSuccess;

	DWORD lastError = GetLastError ();
	if (lastError == ERROR_SUCCESS)
		return kErrorCodeSuccess;
	
	return static_cast<ErrorCode>(lastError);
}

}
}

#endif
