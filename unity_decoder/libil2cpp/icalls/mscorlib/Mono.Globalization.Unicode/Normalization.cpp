#include "il2cpp-config.h"

#include "icalls/mscorlib/Mono.Globalization.Unicode/Normalization.h"
#include "vm/Exception.h"
#include "normalization-tables.h"

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace Mono
{
namespace Globalization
{
namespace Unicode
{
    void Normalization::load_normalization_resource(Il2CppIntPtr* argProps, Il2CppIntPtr* argMappedChars, Il2CppIntPtr* argCharMapIndex, Il2CppIntPtr* argHelperIndex, Il2CppIntPtr* argMapIdxToComposite, Il2CppIntPtr* argCombiningClass)
    {
        argProps->m_value = (uint8_t*)props;
        argMappedChars->m_value = (uint8_t*)mappedChars;
        argCharMapIndex->m_value = (uint8_t*)charMapIndex;
        argHelperIndex->m_value = (uint8_t*)helperIndex;
        argMapIdxToComposite->m_value = (uint8_t*)mapIdxToComposite;
        argCombiningClass->m_value = (uint8_t*)combiningClass;
    }
} /* namespace Unicode */
} /* namespace Globalization */
} /* namespace Mono */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
