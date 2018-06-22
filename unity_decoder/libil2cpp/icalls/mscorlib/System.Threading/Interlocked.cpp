#include "il2cpp-config.h"
#include "icalls/mscorlib/System.Threading/Interlocked.h"
#include <ctype.h>
#include "os/Atomic.h"
#include "os/Mutex.h"
#include "vm/Exception.h"

using namespace il2cpp::os;

union LongDoubleUnion
{
    int64_t l_val;
    double d_val;
};

union IntFloatUnion
{
    int32_t i_val;
    float f_val;
};

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{
namespace Threading
{
#if !IL2CPP_ENABLE_INTERLOCKED_64_REQUIRED_ALIGNMENT
    static FastMutex m_Atomic64Mutex;
#endif

    void* Interlocked::CompareExchange_T(void** location, void* value, void* comparand)
    {
        return Atomic::CompareExchangePointer(location, value, comparand);
    }

    Il2CppIntPtr Interlocked::CompareExchangeIntPtr(Il2CppIntPtr* location, Il2CppIntPtr value, Il2CppIntPtr comparand)
    {
        Il2CppIntPtr result;
        result.m_value = Atomic::CompareExchangePointer(&location->m_value, value.m_value, comparand.m_value);
        return result;
    }

    int32_t Interlocked::CompareExchange(int32_t* location, int32_t value, int32_t comparand)
    {
        return Atomic::CompareExchange(location, value, comparand);
    }

    int64_t Interlocked::CompareExchange64(int64_t* location, int64_t value, int64_t comparand)
    {
#if IL2CPP_ENABLE_INTERLOCKED_64_REQUIRED_ALIGNMENT
        return Atomic::CompareExchange64(location, value, comparand);
#else
        FastAutoLock lock(&m_Atomic64Mutex);
        int64_t orig = *location;
        if (*location == comparand)
            *location = value;

        return orig;
#endif
    }

    int32_t Interlocked::Add(int32_t* location1, int32_t value)
    {
        return Atomic::Add(location1, value);
    }

    int64_t Interlocked::Add64(int64_t* location1, int64_t value)
    {
#if IL2CPP_ENABLE_INTERLOCKED_64_REQUIRED_ALIGNMENT
        return Atomic::Add64(location1, value);
#else
        FastAutoLock lock(&m_Atomic64Mutex);
        return *location1 += value;
#endif
    }

    double Interlocked::CompareExchangeDouble(double* location1, double value, double comparand)
    {
        LongDoubleUnion val, ret, cmp;

        cmp.d_val = comparand;
        val.d_val = value;
        ret.l_val = (int64_t)Atomic::CompareExchange64((int64_t*)location1, val.l_val, cmp.l_val);

        return ret.d_val;
    }

    float Interlocked::CompareExchangeSingle(float* location1, float value, float comparand)
    {
        IntFloatUnion val, ret, cmp;

        cmp.f_val = comparand;
        val.f_val = value;
        ret.i_val = (int32_t)Atomic::CompareExchange((int32_t*)location1, val.i_val, cmp.i_val);

        return ret.f_val;
    }

    int32_t Interlocked::Increment(int32_t* value)
    {
        return Atomic::Increment(value);
    }

    int64_t Interlocked::Increment64(int64_t* location)
    {
#if IL2CPP_ENABLE_INTERLOCKED_64_REQUIRED_ALIGNMENT
        return Atomic::Increment64(location);
#else
        FastAutoLock lock(&m_Atomic64Mutex);
        return ++(*location);
#endif
    }

    int32_t Interlocked::Decrement(int32_t* location)
    {
        return Atomic::Decrement(location);
    }

    int64_t Interlocked::Decrement64(int64_t* location)
    {
#if IL2CPP_ENABLE_INTERLOCKED_64_REQUIRED_ALIGNMENT
        return Atomic::Decrement64(location);
#else
        FastAutoLock lock(&m_Atomic64Mutex);
        return --(*location);
#endif
    }

    double Interlocked::ExchangeDouble(double* location1, double value)
    {
        LongDoubleUnion val, ret;

        val.d_val = value;
        ret.l_val = (int64_t)Atomic::Exchange64((int64_t*)location1, val.l_val);

        return ret.d_val;
    }

    Il2CppIntPtr Interlocked::ExchangeIntPtr(Il2CppIntPtr* location, Il2CppIntPtr value)
    {
        Il2CppIntPtr result;
        result.m_value = Atomic::ExchangePointer(&location->m_value, value.m_value);
        return result;
    }

    int32_t Interlocked::Exchange(int32_t* location1, int32_t value)
    {
        return Atomic::Exchange(location1, value);
    }

    int64_t Interlocked::Exchange64(int64_t* location1, int64_t value)
    {
#if IL2CPP_ENABLE_INTERLOCKED_64_REQUIRED_ALIGNMENT
        return Atomic::Exchange64(location1, value);
#else
        FastAutoLock lock(&m_Atomic64Mutex);
        int64_t orig = *location1;
        *location1 = value;
        return orig;
#endif
    }

    void* Interlocked::ExchangePointer(void** location1, void* value)
    {
        return Atomic::ExchangePointer(location1, value);
    }

    float Interlocked::ExchangeSingle(float* location1, float value)
    {
        IntFloatUnion val, ret;

        val.f_val = value;
        ret.i_val = (int32_t)Atomic::Exchange((int32_t*)location1, val.i_val);

        return ret.f_val;
    }

    int64_t Interlocked::Read(int64_t* location)
    {
#if IL2CPP_ENABLE_INTERLOCKED_64_REQUIRED_ALIGNMENT
        return Atomic::Read64(location);
#else
        FastAutoLock lock(&m_Atomic64Mutex);
        return *location;
#endif
    }

#if NET_4_0
    int32_t Interlocked::CompareExchange(int32_t* location1, int32_t value, int32_t comparand, bool* succeeded)
    {
        int32_t result = CompareExchange(location1, value, comparand);
        *succeeded = result == comparand;
        return result;
    }

#endif
} /* namespace Threading */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
