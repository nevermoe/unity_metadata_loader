#pragma once

#include <cassert>

#ifdef __EMSCRIPTEN__
#include <emscripten/threading.h>
#endif

// Assumes clang or gcc as compiler.
#if IL2CPP_TARGET_POSIX

namespace il2cpp { namespace os
{
	inline int32_t Atomic::Add (volatile int32_t* location1, int32_t value)
	{
		ASSERT_ALIGNMENT (location1, 4);
		return __sync_add_and_fetch (location1, value);
	}

	inline int64_t Atomic::Add64 (volatile int64_t* location1, int64_t value)
	{
		ASSERT_ALIGNMENT (location1, 8);
		return __sync_add_and_fetch (location1, value);
	}

	inline int32_t Atomic::Increment (volatile int32_t* value)
	{
		ASSERT_ALIGNMENT (value, 4);
		return __sync_add_and_fetch (value, 1);
	}

	inline int64_t Atomic::Increment64 (volatile int64_t* value)
	{
		ASSERT_ALIGNMENT (value, 8);
		return __sync_add_and_fetch (value, 1);
	}

	inline int32_t Atomic::Decrement (volatile int32_t* value)
	{
		ASSERT_ALIGNMENT (value, 4);
		return __sync_add_and_fetch (value, -1);
	}

	inline int64_t Atomic::Decrement64 (volatile int64_t* value)
	{
		ASSERT_ALIGNMENT (value, 8);
		return __sync_add_and_fetch (value, -1);
	}

	inline int32_t Atomic::CompareExchange (volatile int32_t* dest, int32_t exchange, int32_t comparand)
	{
		ASSERT_ALIGNMENT (dest, 4);
		return __sync_val_compare_and_swap (dest, comparand, exchange);
	}

	inline int64_t Atomic::CompareExchange64 (volatile int64_t* dest, int64_t exchange, int64_t comparand)
	{
		ASSERT_ALIGNMENT (dest, 8);
#ifdef __EMSCRIPTEN__
		return emscripten_atomic_cas_u64((void*)dest, comparand, exchange) == comparand ? comparand : *dest;
#else
		return __sync_val_compare_and_swap (dest, comparand, exchange);
#endif
	}

	inline void* Atomic::CompareExchangePointer (void* volatile* dest, void* exchange, void* comparand)
	{
		ASSERT_ALIGNMENT (dest, IL2CPP_SIZEOF_VOID_P);
		return __sync_val_compare_and_swap (dest, comparand, exchange);
	}

	inline int32_t Atomic::Exchange (volatile int32_t* dest, int32_t exchange)
	{
		ASSERT_ALIGNMENT (dest, 4);
#ifdef __EMSCRIPTEN__
		return emscripten_atomic_exchange_u32((void*)dest, exchange);
#else
		int32_t prev;
		do { prev = *dest; }
		while (!__sync_bool_compare_and_swap(dest, prev, exchange));
		return prev;
#endif
	}

	inline int64_t Atomic::Exchange64 (volatile int64_t* dest, int64_t exchange)
	{
		ASSERT_ALIGNMENT (dest, 8);
#ifdef __EMSCRIPTEN__
		return emscripten_atomic_exchange_u64((void*)dest, exchange);
#else
		int64_t prev;
		do { prev = *dest; }
		while (!__sync_bool_compare_and_swap(dest, prev, exchange));
		return prev;
#endif
	}

	inline void* Atomic::ExchangePointer (void* volatile* dest, void* exchange)
	{
		ASSERT_ALIGNMENT (dest, IL2CPP_SIZEOF_VOID_P);
#ifdef __EMSCRIPTEN__
		return (void*)emscripten_atomic_exchange_u32((void*)dest, (uint32_t)exchange);
#else
		void* prev;
		do { prev = *dest; }
		while (!__sync_bool_compare_and_swap(dest, prev, exchange));
		return prev;	
#endif
	}

	inline int64_t Atomic::Read64 (volatile int64_t* addr)
	{
		ASSERT_ALIGNMENT (addr, 8);
		return __sync_fetch_and_add (addr, 0);
	}

	inline void Atomic::MemoryBarrier ()
	{
		__sync_synchronize ();
	}
} }

#endif
