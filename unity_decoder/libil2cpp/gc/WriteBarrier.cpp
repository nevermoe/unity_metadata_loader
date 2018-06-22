#include "il2cpp-config.h"
#include "gc/WriteBarrier.h"

namespace il2cpp
{
namespace gc
{
    void WriteBarrier::GenericStore(void* ptr, Il2CppObject* value)
    {
        *(void**)ptr = value;
    }
} /* gc */
} /* il2cpp */
