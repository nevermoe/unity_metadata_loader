#pragma once

#include "os/ErrorCodes.h"

namespace il2cpp
{
namespace os
{
	
class ThreadLocalValueImpl;

class ThreadLocalValue
{
public:
	ThreadLocalValue ();
	~ThreadLocalValue ();
	ErrorCode SetValue (void* value);
	ErrorCode GetValue (void** value);
private:
	ThreadLocalValueImpl* m_ThreadLocalValue;
};

}
}