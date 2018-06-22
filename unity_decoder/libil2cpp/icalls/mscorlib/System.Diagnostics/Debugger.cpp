#include "il2cpp-config.h"

#include <cassert>

#include "icalls/mscorlib/System.Diagnostics/Debugger.h"
#include "os/Debug.h"

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{
namespace Diagnostics
{

// Until we have il2cpp debugger, return whether a native debugger is attached
bool Debugger::IsAttached_internal()
{
	return os::Debug::IsDebuggerPresent();
}

} /* namespace Diagnostics */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
