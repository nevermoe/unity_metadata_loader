#pragma once

#if IL2CPP_THREADS_WIN32

#include "os/ErrorCodes.h"
#include "utils/NonCopyable.h"

#include "WindowsHelpers.h"

namespace il2cpp
{
namespace os
{

class ThreadLocalValueImpl : public il2cpp::utils::NonCopyable
{
public:
	ThreadLocalValueImpl ();
	~ThreadLocalValueImpl ();
	ErrorCode SetValue (void* value);
	ErrorCode GetValue (void** value);
private:
	DWORD m_Index;
};

}
}

#endif
