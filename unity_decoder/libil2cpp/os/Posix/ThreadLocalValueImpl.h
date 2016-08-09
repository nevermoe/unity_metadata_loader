#pragma once

#if IL2CPP_THREADS_PTHREAD

#include "os/ErrorCodes.h"
#include "utils/NonCopyable.h"

#include <pthread.h>

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
	pthread_key_t m_Key;
};

}
}

#endif
