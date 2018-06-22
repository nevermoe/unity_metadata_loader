#include "il2cpp-config.h"

#if IL2CPP_PLATFORM_WIN32

#include "os/LastError.h"

#include "WindowsHeaders.h"

namespace il2cpp
{
namespace os
{

uint32_t LastError::GetLastError()
{
	return ::GetLastError();
}

} /* namespace os */
} /* namespace il2cpp*/

#endif 