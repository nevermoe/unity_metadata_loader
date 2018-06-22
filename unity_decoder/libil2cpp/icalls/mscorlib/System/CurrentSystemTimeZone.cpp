#include "il2cpp-config.h"
#include <string>
#include "icalls/mscorlib/System/CurrentSystemTimeZone.h"
#include "class-internals.h"
#include "object-internals.h"
#include "vm/Array.h"
#include "vm/Exception.h"
#include "vm/String.h"
#include "os/TimeZone.h"

using namespace il2cpp::vm;
using namespace il2cpp::os;

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{
    bool CurrentSystemTimeZone::GetTimeZoneData(int year, Il2CppArray** data, Il2CppArray** names)
    {
        NOT_IMPLEMENTED_NO_ASSERT(CurrentSystemTimeZone::GetTimeZoneData, "Check arguments and write barriers");
        int64_t dataTemp[4] = {0};
        std::string namesTemp[2];
        IL2CPP_CHECK_ARG_NULL(data);
        IL2CPP_CHECK_ARG_NULL(names);

        *data = Array::New(il2cpp_defaults.int64_class, 4);
        //mono_gc_wbarrier_generic_store (data, Array::New (il2cpp_defaults.int64_class, 4));
        *names = Array::New(il2cpp_defaults.string_class, 2);
        //mono_gc_wbarrier_generic_store (names, Array::New (il2cpp_defaults.string_class, 2));

        if (!TimeZone::GetTimeZoneData(year, dataTemp, namesTemp))
            return false;

        for (int i = 0; i < 4; i++)
            il2cpp_array_set((*data), int64_t, i, dataTemp[i]);

        for (int i = 0; i < 2; i++)
            il2cpp_array_setref((*names), i, String::New(namesTemp[i].c_str()));

        return true;
    }
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
