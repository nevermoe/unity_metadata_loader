#if NET_4_0

#include "il2cpp-config.h"
#include "RuntimeMarshal.h"
#include "mono-structs.h"

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace Mono
{
    void RuntimeMarshal::FreeAssemblyName(Il2CppMonoAssemblyName* name)
    {
        IL2CPP_FREE(name->name.m_value);
        IL2CPP_FREE(name->culture.m_value);
        IL2CPP_FREE(name->hash_value.m_value);
        IL2CPP_FREE(name->public_key.m_value);
    }
} // namespace Mono
} // namespace mscorlib
} // namespace icalls
} // namespace il2cpp

#endif
