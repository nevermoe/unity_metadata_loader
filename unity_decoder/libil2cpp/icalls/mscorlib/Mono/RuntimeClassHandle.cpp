#if NET_4_0

#include "il2cpp-config.h"
#include "class-internals.h"
#include "RuntimeClassHandle.h"
#include "vm/Class.h"

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace Mono
{
    Il2CppIntPtr RuntimeClassHandle::GetTypeFromClass(Il2CppClass* klass)
    {
        Il2CppIntPtr retVal;
        retVal.m_value = (void*)il2cpp::vm::Class::GetType(klass);
        return retVal;
    }
} // namespace Mono
} // namespace mscorlib
} // namespace icalls
} // namespace il2cpp

#endif
