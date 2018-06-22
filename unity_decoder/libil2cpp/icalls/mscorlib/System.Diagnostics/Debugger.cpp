#include "il2cpp-config.h"

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

#if NET_4_0
    bool Debugger::IsLogging()
    {
        NOT_IMPLEMENTED_ICALL(Debugger::IsLogging);
        IL2CPP_UNREACHABLE;
        return false;
    }

    void Debugger::Log(int32_t level, Il2CppString* category, Il2CppString* message)
    {
        NOT_IMPLEMENTED_ICALL(Debugger::Log);
        IL2CPP_UNREACHABLE;
    }

#endif
} /* namespace Diagnostics */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
