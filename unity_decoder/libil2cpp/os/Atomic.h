#pragma once

#include <stdint.h>
#include "utils/NonCopyable.h"

namespace il2cpp
{
namespace os
{

// On Windows, MemoryBarrier is defined as a macro on x64. Conflicts with MemoryBarrier in Atomic.h.
#undef MemoryBarrier
	
class Atomic : public il2cpp::utils::NonCopyable
{
public:
	// All 32bit atomics must be performed on 4-byte aligned addresses. All 64bit atomics must be
	// performed on 8-byte aligned addresses.
	
	// Add and Add64 return the *result* of the addition, not the old value! (i.e. they work like
	// InterlockedAdd and __sync_add_and_fetch).
	static inline int32_t Add (volatile int32_t* location1, int32_t value);
	static inline int64_t Add64 (volatile int64_t* location1, int64_t value);
	static inline int32_t Increment (volatile int32_t* value);
	static inline int64_t Increment64 (volatile int64_t* value);
	static inline int32_t Decrement (volatile int32_t* value);
	static inline int64_t Decrement64 (volatile int64_t* value);
	static inline int32_t CompareExchange (volatile int32_t* dest, int32_t exchange, int32_t comparand);
	static inline int64_t CompareExchange64 (volatile int64_t* dest, int64_t exchange, int64_t comparand);
	static inline void* CompareExchangePointer (void* volatile* dest, void* exchange, void* comparand);
	static inline int32_t Exchange (volatile int32_t* dest, int32_t exchange);
	static inline int64_t Exchange64 (volatile int64_t* dest, int64_t exchange);
	static inline void* ExchangePointer (void* volatile* dest, void* exchange);
	static inline int64_t Read64 (volatile int64_t* addr);
	static inline void MemoryBarrier ();
	
	static inline uint32_t Add (volatile uint32_t* location1, uint32_t value)
	{
		return static_cast<uint32_t> (Add ((volatile int32_t*) location1, (int32_t) value));
	}

	template<typename T>
	static inline T* CompareExchangePointer (T* volatile* dest, T* newValue, T* oldValue)
	{
		return static_cast<T*> (CompareExchangePointer ((void* volatile*) dest, newValue, oldValue));
	}

	template<typename T>
	static inline T* ExchangePointer (T* volatile* dest, T* newValue)
	{
		return static_cast<T*> (ExchangePointer ((void* volatile*) dest, newValue));
	}
	
	static inline uint64_t Read64 (volatile uint64_t* addr)
	{
		return static_cast<uint64_t> (Read64 ((volatile int64_t*) addr));
	}

	template<typename T>
	static inline T* ReadPointer (T* volatile* pointer)
	{
		#if IL2CPP_SIZEOF_VOID_P == 4
		return reinterpret_cast<T*> (Add (reinterpret_cast<volatile int32_t*> (pointer), 0));
		#else
		return reinterpret_cast<T*> (Add64 (reinterpret_cast<volatile int64_t*> (pointer), 0));
		#endif
	}

	static inline uint32_t Increment (volatile uint32_t* value)
	{
		return static_cast<uint32_t> (Increment (reinterpret_cast<volatile int32_t*> (value)));
	}
	static inline uint64_t Increment64 (volatile uint64_t* value)
	{
		return static_cast<uint64_t> (Increment64 (reinterpret_cast<volatile int64_t*> (value)));
	}
	static inline uint32_t Decrement (volatile uint32_t* value)
	{
		return static_cast<uint32_t> (Decrement (reinterpret_cast<volatile int32_t*> (value)));
	}
	static inline uint64_t Decrement64 (volatile uint64_t* value)
	{
		return static_cast<uint64_t> (Decrement64 (reinterpret_cast<volatile int64_t*> (value)));
	}
	static inline uint32_t CompareExchange (volatile uint32_t* value, uint32_t newValue, uint32_t oldValue)
	{
		return static_cast<uint32_t> (CompareExchange (reinterpret_cast<volatile int32_t*> (value), newValue, oldValue));
	}
	static inline uint64_t CompareExchange64 (volatile uint64_t* value, uint64_t newValue, uint64_t oldValue)
	{
		return static_cast<uint64_t> (CompareExchange64 (reinterpret_cast<volatile int64_t*> (value), newValue, oldValue));
	}
	static inline uint32_t Exchange (volatile uint32_t* value, uint32_t newValue)
	{
		return static_cast<uint32_t> (Exchange (reinterpret_cast<volatile int32_t*> (value), newValue));
	}
	static inline uint64_t Exchange64 (volatile uint64_t* value, uint64_t newValue)
	{
		return static_cast<uint64_t> (Exchange64 (reinterpret_cast<volatile int64_t*> (value), newValue));
	}
};

}
}

#if !IL2CPP_SUPPORT_THREADS

namespace il2cpp
{
namespace os
{

inline int32_t Atomic::Add (volatile int32_t* location1, int32_t value)
{
	return *location1 += value;
}

inline int64_t Atomic::Add64 (volatile int64_t* location1, int64_t value)
{
	return *location1 += value;
}

inline int32_t Atomic::Increment (volatile int32_t* value)
{
	return ++(*value);
}

inline int64_t Atomic::Increment64 (volatile int64_t* value)
{
	return ++(*value);
}

inline int32_t Atomic::Decrement (volatile int32_t* value)
{
	return --(*value);
}

inline int64_t Atomic::Decrement64 (volatile int64_t* value)
{
	return --(*value);
}

inline int32_t Atomic::CompareExchange (volatile int32_t* dest, int32_t exchange, int32_t comparand)
{
	int32_t orig = *dest;
	if (*dest == comparand)
		*dest = exchange;

	return orig;
}

inline int64_t Atomic::CompareExchange64 (volatile int64_t* dest, int64_t exchange, int64_t comparand)
{
	int64_t orig = *dest;
	if (*dest == comparand)
		*dest = exchange;

	return orig;
}

inline void* Atomic::CompareExchangePointer (void* volatile* dest, void* exchange, void* comparand)
{
	void* orig = *dest;
	if (*dest == comparand)
		*dest = exchange;

	return orig;
}

inline int64_t Atomic::Exchange64 (volatile int64_t* dest, int64_t exchange)
{
	int64_t orig = *dest;
	*dest = exchange;
	return orig;
}

inline int32_t Atomic::Exchange (volatile int32_t* dest, int32_t exchange)
{
	int32_t orig = *dest;
	*dest = exchange;
	return orig;
}

inline void* Atomic::ExchangePointer (void* volatile* dest, void* exchange)
{
	void* orig = *dest;
	*dest = exchange;
	return orig;
}

int64_t Atomic::Read64 (volatile int64_t* addr)
{
	return *addr;
}

inline void Atomic::MemoryBarrier ()
{
	// Do nothing.
}

}
}

#elif IL2CPP_TARGET_WINDOWS
#include "os/Win32/AtomicImpl.h"
#elif IL2CPP_TARGET_PS4	
#include "os/AtomicImpl.h"	// has to come earlier than posix 
#elif IL2CPP_TARGET_PSP2
#include "os/PSP2/AtomicImpl.h"
#elif IL2CPP_TARGET_POSIX
#include "os/Posix/AtomicImpl.h"
#elif IL2CPP_TARGET_XBOXONE
#include "os/Win32/AtomicImpl.h"
#else
#include "os/AtomicImpl.h"
#endif
