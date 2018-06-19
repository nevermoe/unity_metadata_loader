#include "il2cpp-config.h"

#if IL2CPP_PLATFORM_WIN32

#include "os/Debug.h"
#include <windows.h>

namespace il2cpp
{
namespace os
{

bool Debug::IsDebuggerPresent()
{
	return ::IsDebuggerPresent();
}

}
}

#endif
