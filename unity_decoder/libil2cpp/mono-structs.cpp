#include "il2cpp-config.h"
#include "mono-structs.h"
#include "utils/Memory.h"

GPtrArray* void_ptr_array_to_gptr_array(const VoidPtrArray& array)
{
    GPtrArray *pRetVal;

    pRetVal = (GPtrArray*)IL2CPP_MALLOC(sizeof(GPtrArray));

    pRetVal->len = (unsigned int)array.size();
    if (pRetVal->len > 0)
    {
        size_t numBytes = sizeof(void*) * pRetVal->len;
        pRetVal->pdata = IL2CPP_MALLOC(numBytes);
        memcpy(pRetVal->pdata, array.data(), numBytes);
    }
    else
    {
        pRetVal->pdata = NULL;
    }

    return pRetVal;
}

GPtrArray* empty_gptr_array()
{
    GPtrArray *pRetVal = (GPtrArray*)IL2CPP_MALLOC(sizeof(GPtrArray));
    pRetVal->len = 0;
    pRetVal->pdata = NULL;
    return pRetVal;
}

void free_gptr_array(GPtrArray *pArray)
{
    if (!pArray)
        return;

    if (pArray->pdata)
    {
        IL2CPP_FREE(pArray->pdata);
        pArray->pdata = NULL;
    }

    IL2CPP_FREE(pArray);
}
