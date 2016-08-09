#pragma once

#define ASM_LABEL(i)    #i ":\n\t"

#define ATOMIC_LOAD(PRE, POST) \
	atomic_word res; \
	__asm__ __volatile__ \
	( \
        PRE \
		"ldr	%0, %1\n\t" \
        POST \
		: "=r" (res) \
		: "m" (*p) \
        : "memory" \
	); \
	return res;

#define ATOMIC_LOAD2(PRE, POST) \
    atomic_word2 ret; \
	atomic_word res; \
	__asm__ __volatile__ \
	( \
        PRE \
		"ldr	%0, %1\n\t" \
        POST \
		: "=r" (res) \
		: "m" (*p) \
        : "memory" \
	); \
    ret.v = res; \
	return ret;

#define ATOMIC_STORE(PRE, POST) \
	__asm__ __volatile__ \
	( \
        PRE \
		"str	%1, %0\n\t" \
        POST \
		: "=m" (*p) \
		: "r" (v) \
		: "memory" \
	);

#define ATOMIC_XCHG(PRE, POST) \
	atomic_word res; \
	atomic_word success; \
	__asm__ __volatile__ \
	( \
        PRE \
	ASM_LABEL (0) \
		"ldrex	%2, [%4]\n\t" \
		"strex	%0, %3, [%4]\n\t" \
		"teq	%0, #0\n\t" \
		"bne	0b\n\t" \
        POST \
		: "=&r" (success), "+m" (*p), "=&r" (res) \
		: "r" (v), "r" (p) \
		: "cc", "memory" \
	); \
	return res;

#define ATOMIC_XCHG2(PRE, POST) \
    atomic_word2 ret; \
	atomic_word res; \
	atomic_word success; \
	__asm__ __volatile__ \
	( \
        PRE \
	ASM_LABEL (0) \
		"ldrex	%2, [%4]\n\t" \
		"strex	%0, %3, [%4]\n\t" \
		"teq	%0, #0\n\t" \
		"bne	0b\n\t" \
        POST \
		: "=&r" (success), "+m" (*p), "=&r" (res) \
		: "r" (v), "r" (p) \
		: "cc", "memory" \
	); \
    ret.v = res; \
	return ret;

static inline void atomic_thread_fence (int)
{
	__sync_synchronize();
}

static inline atomic_word atomic_load_explicit (const volatile atomic_word* p, int)
{
    ATOMIC_LOAD ("", "")
}

static inline void atomic_store_explicit (volatile atomic_word* p, atomic_word v, int)
{
    ATOMIC_STORE ("", "")
}

static inline atomic_word atomic_exchange_explicit (volatile atomic_word* p, atomic_word v, int)
{
    ATOMIC_XCHG ("", "")
}

static inline bool atomic_compare_exchange_strong_explicit (volatile atomic_word* p, atomic_word* oldval, atomic_word newval, int, int)
{
	return __sync_val_compare_and_swap(p, oldval, newval);
}

static inline bool atomic_compare_exchange_weak_explicit (volatile atomic_word* p, atomic_word* oldval, atomic_word newval, int, int)
{
	return __sync_val_compare_and_swap(p, oldval, newval);
}

static inline atomic_word atomic_fetch_add_explicit(volatile atomic_word *p, atomic_word val, int)
{
	return __sync_fetch_and_add(p, val);
}

static inline atomic_word atomic_fetch_sub_explicit(volatile atomic_word *p, atomic_word val, int)
{
	return __sync_fetch_and_add(p, -val);
}

/*
 *  extensions
 */

static inline void atomic_retain(volatile int *p)
{
	__sync_fetch_and_add(p, 1);
}

static inline bool atomic_release(volatile int *p)
{
	return __sync_fetch_and_add(p, -1) == 1;
}

// double word

static inline atomic_word2 atomic_load_explicit (const volatile atomic_word2* p, int)
{
    ATOMIC_LOAD2 ("", "")
}

static inline void atomic_store_explicit (volatile atomic_word2* p, atomic_word2 v, int)
{
    ATOMIC_STORE ("", "")
}

static inline atomic_word2 atomic_exchange_explicit (volatile atomic_word2* p, atomic_word2 v, int)
{
    ATOMIC_XCHG2 ("", "")
}

static inline bool atomic_compare_exchange_strong_explicit (volatile atomic_word2* p, atomic_word2* oldval, atomic_word2 newval, int, int)
{
	return __sync_val_compare_and_swap(&p->v, &oldval->v, newval.v);
}

