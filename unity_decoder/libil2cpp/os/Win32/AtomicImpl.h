#pragma once

#if IL2CPP_PLATFORM_WIN32

#include "os/Win32/WindowsHeaders.h"

namespace il2cpp
{
namespace os
{

inline int32_t Atomic::Add (volatile int32_t* location1, int32_t value)
{
	return (InterlockedExchangeAdd ((long volatile*)location1, value) + value);
}

inline int64_t Atomic::Add64 (volatile int64_t* location1, int64_t value)
{
	return (InterlockedExchangeAdd64 ((long long volatile*)location1, value) + value);
}

inline int32_t Atomic::Increment (volatile int32_t* value)
{
	return InterlockedIncrement ((long volatile*)value);
}

inline int64_t Atomic::Increment64 (volatile int64_t* value)
{
	return InterlockedIncrement64 (value);
}

inline int32_t Atomic::Decrement (volatile int32_t* value)
{
	return InterlockedDecrement ((long volatile*)value);
}

inline int64_t Atomic::Decrement64 (volatile int64_t* value)
{
	return InterlockedDecrement64 ((long long volatile*)value);
}

inline int32_t Atomic::CompareExchange (volatile int32_t* dest, int32_t exchange, int32_t comparand)
{
	return InterlockedCompareExchange ((long volatile*)dest, exchange, comparand);
}

inline int64_t Atomic::CompareExchange64 (volatile int64_t* dest, int64_t exchange, int64_t comparand)
{
	return InterlockedCompareExchange64 ((long long volatile*)dest, exchange, comparand);
}

inline void* Atomic::CompareExchangePointer (void* volatile* dest, void* exchange, void* comparand)
{
	return InterlockedCompareExchangePointer (dest, exchange, comparand);
}

inline int64_t Atomic::Exchange64 (volatile int64_t* dest, int64_t exchange)
{
	return InterlockedExchange64 (dest, exchange);
}

inline int32_t Atomic::Exchange (volatile int32_t* dest, int32_t exchange)
{
	return InterlockedExchange ((long volatile*)dest, exchange);
}

inline void* Atomic::ExchangePointer (void* volatile* dest, void* exchange)
{
	return InterlockedExchangePointer (dest, exchange);
}

int64_t Atomic::Read64 (volatile int64_t* addr)
{
	return InterlockedCompareExchange64 ((long long volatile*)addr, 0, 0);
}

// MemoryBarrier is defined as a macro on x64 and as a function on x86.
#undef MemoryBarrier
inline void Atomic::MemoryBarrier ()
{
#if defined(_AMD64_)
	::__faststorefence ();
#elif defined(_M_IX86)
	::MemoryBarrier ();
#elif defined(_M_ARM)
	__dmb(_ARM_BARRIER_SY);
#else
#error Not implemented;
#endif
}

}
}

#endif
