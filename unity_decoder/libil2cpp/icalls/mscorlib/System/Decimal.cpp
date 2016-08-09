#include "il2cpp-config.h"
#include <cassert>
#include "object-internals.h"
#include "class-internals.h"
#include "vm/String.h"
#include "icalls/mscorlib/System/Decimal.h"
#include "vm/Exception.h"

#define DECIMAL_MAX_SCALE 28
#define DECIMAL_MAX_INTFACTORS 9

#define DECIMAL_SUCCESS 0
#define DECIMAL_FINISHED 1
#define DECIMAL_OVERFLOW 2
#define DECIMAL_INVALID_CHARACTER 2
#define DECIMAL_INTERNAL_ERROR 3
#define DECIMAL_INVALID_BITS 4
#define DECIMAL_DIVIDE_BY_ZERO 5
#define DECIMAL_BUFFER_OVERFLOW 6

#define LIT_GUINT32(x) x
#define LIT_GUINT64(x) x##LL

#define LIT_GUINT32_HIGHBIT 0x80000000
#define LIT_GUINT64_HIGHBIT LIT_GUINT64(0x8000000000000000)

#define DECIMAL_LOG_NEGINF -1000

#define DECINIT(src) memset(src, 0, sizeof(il2cpp_decimal_repr))

#define DECCOPY(dest, src) memcpy(dest, src, sizeof(il2cpp_decimal_repr))

#define DECNEGATE(p1) (p1)->u.signscale.sign = 1 - (p1)->u.signscale.sign

#define DECTO128(pd, lo, hi) \
	lo = (((uint64_t)(pd)->mid32) << 32) | (pd)->lo32; \
    hi = (pd)->hi32;

#ifndef MAX
#define MAX(a,b) (((a)>(b)) ? (a) : (b))
#endif

#ifdef _DEBUG
#include <assert.h>
#define PRECONDITION(flag) assert(flag)
#define POSTCONDITION(flag) assert(flag)
#else
#define PRECONDITION(flag)  
#define POSTCONDITION(flag)  
#endif /* _DEBUG */

typedef struct {
	uint64_t lo;
	uint64_t hi;
} dec128_repr;

#define LIT_DEC128(hi, mid, lo) { (((uint64_t)mid)<<32 | lo), hi }

static const dec128_repr dec128decadeFactors[DECIMAL_MAX_SCALE + 1] = {
	LIT_DEC128(0, 0, 1u), /* == 1 */
	LIT_DEC128(0, 0, 10u), /* == 10 */
	LIT_DEC128(0, 0, 100u), /* == 100 */
	LIT_DEC128(0, 0, 1000u), /* == 1e3m */
	LIT_DEC128(0, 0, 10000u), /* == 1e4m */
	LIT_DEC128(0, 0, 100000u), /* == 1e5m */
	LIT_DEC128(0, 0, 1000000u), /* == 1e6m */
	LIT_DEC128(0, 0, 10000000u), /* == 1e7m */
	LIT_DEC128(0, 0, 100000000u), /* == 1e8m */
	LIT_DEC128(0, 0, 1000000000u), /* == 1e9m */
	LIT_DEC128(0, 2u, 1410065408u), /* == 1e10m */
	LIT_DEC128(0, 23u, 1215752192u), /* == 1e11m */
	LIT_DEC128(0, 232u, 3567587328u), /* == 1e12m */
	LIT_DEC128(0, 2328u, 1316134912u), /* == 1e13m */
	LIT_DEC128(0, 23283u, 276447232u), /* == 1e14m */
	LIT_DEC128(0, 232830u, 2764472320u), /* == 1e15m */
	LIT_DEC128(0, 2328306u, 1874919424u), /* == 1e16m */
	LIT_DEC128(0, 23283064u, 1569325056u), /* == 1e17m */
	LIT_DEC128(0, 232830643u, 2808348672u), /* == 1e18m */
	LIT_DEC128(0, 2328306436u, 2313682944u), /* == 1e19m */
	LIT_DEC128(5u, 1808227885u, 1661992960u), /* == 1e20m */
	LIT_DEC128(54u, 902409669u, 3735027712u), /* == 1e21m */
	LIT_DEC128(542u, 434162106u, 2990538752u), /* == 1e22m */
	LIT_DEC128(5421u, 46653770u, 4135583744u), /* == 1e23m */
	LIT_DEC128(54210u, 466537709u, 2701131776u), /* == 1e24m */
	LIT_DEC128(542101u, 370409800u, 1241513984u), /* == 1e25m */
	LIT_DEC128(5421010u, 3704098002u, 3825205248u), /* == 1e26m */
	LIT_DEC128(54210108u, 2681241660u, 3892314112u), /* == 1e27m */
	LIT_DEC128(542101086u, 1042612833u, 268435456u), /* == 1e28m */
};

static const uint32_t constantsDecadeInt32Factors[DECIMAL_MAX_INTFACTORS + 1] = {
	1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000
};

static int pack128toDecimal(il2cpp_decimal_repr* pA, uint64_t alo, uint64_t ahi, int scale, int sign)
{
	if (scale < 0 || scale > DECIMAL_MAX_SCALE || (ahi >> 32) != 0)
		return DECIMAL_OVERFLOW;

	pA->lo32 = (uint32_t)alo;
	pA->mid32 = (uint32_t)(alo >> 32);
	pA->hi32 = (uint32_t)ahi;
	pA->u.signscale.sign = sign;
	pA->u.signscale.scale = scale;

	return DECIMAL_SUCCESS;
}

static void add128(uint64_t alo, uint64_t ahi, uint64_t blo, uint64_t bhi, uint64_t* pclo, uint64_t* pchi)
{
	alo += blo;
	if (alo < blo) 
		ahi++;
	ahi += bhi;

	*pclo = alo;
	*pchi = ahi;
}

static void sub128(uint64_t alo, uint64_t ahi, uint64_t blo, uint64_t bhi, uint64_t* pclo, uint64_t* pchi)
{
	uint64_t clo, chi;

	clo = alo - blo;
	chi = ahi - bhi;
	if (alo < blo) chi--; /* borrow */

	*pclo = clo;
	*pchi = chi;
}

static int div128by32(uint64_t* plo, uint64_t* phi, uint32_t factor, uint32_t* pRest)
{
	uint64_t a, b, c, h;

	h = *phi;
	a = (uint32_t)(h >> 32);
	b = a / factor;
	a -= b * factor;
	a <<= 32;
	a |= (uint32_t)h;
	c = a / factor;
	a -= c * factor;
	a <<= 32;
	*phi = b << 32 | (uint32_t)c;

	h = *plo;
	a |= (uint32_t)(h >> 32);
	b = a / factor;
	a -= b * factor;
	a <<= 32;
	a |= (uint32_t)h;
	c = a / factor;
	a -= c * factor;
	*plo = b << 32 | (uint32_t)c;

	if (pRest) *pRest = (uint32_t)a;

	a <<= 1;
	return (a >= factor || (a == factor && (c & 1) == 1)) ? 1 : 0;
}

static int mult128by32(uint64_t* pclo, uint64_t* pchi, uint32_t factor, int roundBit)
{
	uint64_t a;
	uint32_t h0, h1;

	a = ((uint64_t)(uint32_t)(*pclo)) * factor;
	if (roundBit) a += factor / 2;
	h0 = (uint32_t)a;

	a >>= 32;
	a += (*pclo >> 32) * factor;
	h1 = (uint32_t)a;

	*pclo = ((uint64_t)h1) << 32 | h0;

	a >>= 32;
	a += ((uint64_t)(uint32_t)(*pchi)) * factor;
	h0 = (uint32_t)a;

	a >>= 32;
	a += (*pchi >> 32) * factor;
	h1 = (uint32_t)a;

	*pchi = ((uint64_t)h1) << 32 | h0;

	return ((a >> 32) == 0) ? DECIMAL_SUCCESS : DECIMAL_OVERFLOW;
}

static void mult96by96to192(uint32_t alo, uint32_t ami, uint32_t ahi, uint32_t blo, uint32_t bmi, uint32_t bhi, uint64_t* pclo, uint64_t* pcmi, uint64_t* pchi)
{
	uint64_t a, b, c, d;
	uint32_t h0, h1, h2, h3, h4, h5;
	int carry0, carry1;

	a = ((uint64_t)alo) * blo;
	h0 = (uint32_t)a;

	a >>= 32; carry0 = 0;
	b = ((uint64_t)alo) * bmi;
	c = ((uint64_t)ami) * blo;
	a += b; if (a < b) carry0++;
	a += c; if (a < c) carry0++;
	h1 = (uint32_t)a;

	a >>= 32; carry1 = 0;
	b = ((uint64_t)alo) * bhi;
	c = ((uint64_t)ami) * bmi;
	d = ((uint64_t)ahi) * blo;
	a += b; if (a < b) carry1++;
	a += c; if (a < c) carry1++;
	a += d; if (a < d) carry1++;
	h2 = (uint32_t)a;

	a >>= 32; a += carry0; carry0 = 0;
	b = ((uint64_t)ami) * bhi;
	c = ((uint64_t)ahi) * bmi;
	a += b; if (a < b) carry0++;
	a += c; if (a < c) carry0++;
	h3 = (uint32_t)a;

	a >>= 32; a += carry1;
	b = ((uint64_t)ahi) * bhi;
	a += b;
	h4 = (uint32_t)a;

	a >>= 32; a += carry0;
	h5 = (uint32_t)a;

	*pclo = ((uint64_t)h1) << 32 | h0;
	*pcmi = ((uint64_t)h3) << 32 | h2;
	*pchi = ((uint64_t)h5) << 32 | h4;
}

static void div192by32(uint64_t* plo, uint64_t* pmi, uint64_t* phi, uint32_t factor)
{
	uint64_t a, b, c, h;

	h = *phi;
	a = (uint32_t)(h >> 32);
	b = a / factor;
	a -= b * factor;
	a <<= 32;
	a |= (uint32_t)h;
	c = a / factor;
	a -= c * factor;
	a <<= 32;
	*phi = b << 32 | (uint32_t)c;

	h = *pmi;
	a |= (uint32_t)(h >> 32);
	b = a / factor;
	a -= b * factor;
	a <<= 32;
	a |= (uint32_t)h;
	c = a / factor;
	a -= c * factor;
	a <<= 32;
	*pmi = b << 32 | (uint32_t)c;

	h = *plo;
	a |= (uint32_t)(h >> 32);
	b = a / factor;
	a -= b * factor;
	a <<= 32;
	a |= (uint32_t)h;
	c = a / factor;
	a -= c * factor;
	a <<= 32;
	*plo = b << 32 | (uint32_t)c;
}

static int incMultConstant128(uint64_t* palo, uint64_t* pahi, int idx, int factor)
{
	uint64_t blo, bhi, h;

	assert(idx >= 0 && idx <= DECIMAL_MAX_SCALE);
	assert(factor > 0 && factor <= 9);

	blo = dec128decadeFactors[idx].lo;
	h = bhi = dec128decadeFactors[idx].hi;
	if (factor != 1) {
		mult128by32(&blo, &bhi, factor, 0);
		if (h > bhi)
			return DECIMAL_OVERFLOW;
	}
	h = *pahi;
	add128(*palo, *pahi, blo, bhi, palo, pahi);
	if (h > *pahi)
		return DECIMAL_OVERFLOW;

	return DECIMAL_SUCCESS;
}

static void roundUp128(uint64_t* pclo, uint64_t* pchi)
{
	if (++(*pclo) == 0) ++(*pchi);
}

static int decimalIsZero(il2cpp_decimal_repr* pA)
{
	return (pA->lo32 == 0 && pA->mid32 == 0 && pA->hi32 == 0);
}

static void sub192(uint64_t alo, uint64_t ami, uint64_t ahi ,uint64_t blo, uint64_t bmi, uint64_t bhi, uint64_t* pclo, uint64_t* pcmi, uint64_t* pchi)
{
	uint64_t clo, cmi, chi;

	clo = alo - blo;
	cmi = ami - bmi;
	chi = ahi - bhi;
	if (alo < blo)
	{
		if (cmi == 0)
			chi--; /* borrow mid */
		cmi--; /* borrow low */
	}
	if (ami < bmi)
		chi--; /* borrow mid */
	*pclo = clo;
	*pcmi = cmi;
	*pchi = chi;
}

static void mult96by32to128(uint32_t alo, uint32_t ami, uint32_t ahi, uint32_t factor, uint64_t* pclo, uint64_t* pchi)
{
	uint64_t a;
	uint32_t h0, h1;

	a = ((uint64_t)alo) * factor;
	h0 = (uint32_t)a;

	a >>= 32;
	a += ((uint64_t)ami) * factor;
	h1 = (uint32_t)a;

	a >>= 32;
	a += ((uint64_t)ahi) * factor;

	*pclo = ((uint64_t)h1) << 32 | h0;
	*pchi = a;
}

static void add192(uint64_t alo, uint64_t ami, uint64_t ahi, uint64_t blo, uint64_t bmi, uint64_t bhi, uint64_t* pclo, uint64_t* pcmi, uint64_t* pchi)
{
	alo += blo;
	if (alo < blo)
	{ /* carry low */
		ami++;
		if (ami == 0)
			ahi++; /* carry mid */
	}
	ami += bmi;
	if (ami < bmi)
		ahi++; /* carry mid */
	ahi += bhi;
	*pclo = alo;
	*pcmi = ami;
	*pchi = ahi;
}

static uint32_t div192by96to32withRest(uint64_t* palo, uint64_t* pami, uint64_t* pahi, uint32_t blo, uint32_t bmi, uint32_t bhi)
{
	uint64_t rlo, rmi, rhi; /* remainder */
	uint64_t tlo, thi; /* term */
	uint32_t c;

	rlo = *palo; rmi = *pami; rhi = *pahi;
	if (rhi >= (((uint64_t)bhi) << 32)) {
		c = LIT_GUINT32(0xFFFFFFFF);
	}
	else {
		c = (uint32_t)(rhi / bhi);
	}
	mult96by32to128(blo, bmi, bhi, c, &tlo, &thi);
	sub192(rlo, rmi, rhi, 0, tlo, thi, &rlo, &rmi, &rhi);
	while (((int64_t)rhi) < 0) {
		c--;
		add192(rlo, rmi, rhi, 0, (((uint64_t)bmi) << 32) | blo, bhi, &rlo, &rmi, &rhi);
	}
	*palo = rlo; *pami = rmi; *pahi = rhi;

	POSTCONDITION(rhi >> 32 == 0);

	return c;
}

static void div192by96to128(uint64_t alo, uint64_t ami, uint64_t ahi, uint32_t blo, uint32_t bmi, uint32_t bhi, uint64_t* pclo, uint64_t* pchi)
{
	uint64_t rlo, rmi, rhi; /* remainder */
	uint32_t h, c;

	PRECONDITION(ahi < (((uint64_t)bhi) << 32 | bmi)
		|| (ahi == (((uint64_t)bhi) << 32 | bmi) && (ami >> 32) > blo));

	/* high 32 bit*/
	rlo = alo; rmi = ami; rhi = ahi;
	h = div192by96to32withRest(&rlo, &rmi, &rhi, blo, bmi, bhi);

	/* mid 32 bit*/
	rhi = (rhi << 32) | (rmi >> 32); rmi = (rmi << 32) | (rlo >> 32); rlo <<= 32;
	*pchi = (((uint64_t)h) << 32) | div192by96to32withRest(&rlo, &rmi, &rhi, blo, bmi, bhi);

	/* low 32 bit */
	rhi = (rhi << 32) | (rmi >> 32); rmi = (rmi << 32) | (rlo >> 32); rlo <<= 32;
	h = div192by96to32withRest(&rlo, &rmi, &rhi, blo, bmi, bhi);

	/* estimate lowest 32 bit (two last bits may be wrong) */
	if (rhi >= bhi)
	{
		c = LIT_GUINT32(0xFFFFFFFF);
	}
	else
	{
		rhi <<= 32;
		c = (uint32_t)(rhi / bhi);
	}
	*pclo = (((uint64_t)h) << 32) | c;
}

static void rshift128(uint64_t* pclo, uint64_t* pchi)
{
	*pclo >>= 1;
	*pclo |= (*pchi & 1) << 63;
	*pchi >>= 1;
}

static void lshift128(uint64_t* pclo, uint64_t* pchi)
{
	*pchi <<= 1;
	*pchi |= (*pclo & LIT_GUINT64_HIGHBIT) >> 63;
	*pclo <<= 1;
}

static inline int
my_g_bit_nth_msf(uintptr_t mask)
{
	int i;

	// Mono uses some architecture and OS-specific code to implement this function.
	// This is the code from the #else case, which attempts to use sizeof(gsize),
	// which is sizeof(uintptr_t) for us. This seems to be wrong on 64-bit though,
	// So we will explicitly always use 4 here.
	//i = sizeof(gsize) * 8;
	i = 4 * 8;
	while (i > 0) {
		i--;
		if (mask & static_cast<uintptr_t>(1ULL << i))
			return i;
	}
	return -1;
}

static int decimalDivSub(il2cpp_decimal_repr* pA, il2cpp_decimal_repr* pB, uint64_t* pclo, uint64_t* pchi, int* pExp)
{
	uint64_t alo, ami, ahi;
	uint64_t tlo, tmi, thi;
	uint32_t blo, bmi, bhi;
	int ashift, bshift, extraBit, texp;

	ahi = (((uint64_t)(pA->hi32)) << 32) | pA->mid32;
	ami = ((uint64_t)(pA->lo32)) << 32;
	alo = 0;
	blo = pB->lo32;
	bmi = pB->mid32;
	bhi = pB->hi32;

	if (blo == 0 && bmi == 0 && bhi == 0)
	{
		return DECIMAL_DIVIDE_BY_ZERO;
	}

	if (ami == 0 && ahi == 0)
	{
		*pclo = *pchi = 0;
		return DECIMAL_FINISHED;
	}

	/* enlarge dividend to get maximal precision */
	if (ahi == 0)
	{
		ahi = ami;
		ami = 0;
		for (ashift = 64; (ahi & LIT_GUINT64_HIGHBIT) == 0; ++ashift)
		{
			ahi <<= 1;
		}
	}
	else
	{
		for (ashift = 0; (ahi & LIT_GUINT64_HIGHBIT) == 0; ++ashift)
		{
			lshift128(&ami, &ahi);
		}
	}

	/* ensure that divisor is at least 2^95 */
	if (bhi == 0)
	{
		if (bmi == 0)
		{
			uint32_t hi_shift;
			bhi = blo;
			bmi = 0;
			blo = 0;

			//g_assert (g_bit_nth_msf (bhi, 32) == my_g_bit_nth_msf (bhi));

			hi_shift = 31 - my_g_bit_nth_msf(bhi);
			bhi <<= hi_shift;
			bshift = 64 + hi_shift;
		}
		else
		{
			bhi = bmi;
			bmi = blo;
			blo = 0;

			for (bshift = 32; (bhi & LIT_GUINT32_HIGHBIT) == 0; ++bshift)
			{
				bhi <<= 1;
				bhi |= (bmi & LIT_GUINT32_HIGHBIT) >> 31;
				bmi <<= 1;
			}
		}
	}
	else
	{
		for (bshift = 0; (bhi & LIT_GUINT32_HIGHBIT) == 0; ++bshift)
		{
			bhi <<= 1;
			bhi |= (bmi & LIT_GUINT32_HIGHBIT) >> 31;
			bmi <<= 1;
			bmi |= (blo & LIT_GUINT32_HIGHBIT) >> 31;
			blo <<= 1;
		}
	}

	thi = ((uint64_t)bhi) << 32 | bmi;
	tmi = ((uint64_t)blo) << 32;
	tlo = 0;
	if (ahi > thi || (ahi == thi && ami >= tmi))
	{
		sub192(alo, ami, ahi, tlo, tmi, thi, &alo, &ami, &ahi);
		extraBit = 1;
	}
	else
	{
		extraBit = 0;
	}

	div192by96to128(alo, ami, ahi, blo, bmi, bhi, pclo, pchi);
	texp = 128 + ashift - bshift;

	if (extraBit)
	{
		rshift128(pclo, pchi);
		*pchi += LIT_GUINT64_HIGHBIT;
		texp--;
	}

	/* try loss free right shift */
	while (texp > 0 && (*pclo & 1) == 0)
	{
		/* right shift */
		rshift128(pclo, pchi);
		texp--;
	}

	*pExp = texp;

	return DECIMAL_SUCCESS;
}

static int log2_32(uint32_t a)
{
	if (a == 0) return DECIMAL_LOG_NEGINF;

	return my_g_bit_nth_msf (a) + 1;
}

static int log2_64(uint64_t a)
{
    if (a == 0) return DECIMAL_LOG_NEGINF;

#if SIZEOF_VOID_P == 8
	return my_g_bit_nth_msf (a) + 1;
#else
	if ((a >> 32) == 0)
		return my_g_bit_nth_msf ((uint32_t)a) + 1;
	else
		return my_g_bit_nth_msf ((uint32_t)(a >> 32)) + 1 + 32;
#endif
}

static int log2_128(uint64_t alo, uint64_t ahi)
{
    if (ahi == 0) return log2_64(alo);
    else return log2_64(ahi) + 64;
}

static int log2withScale_128(uint64_t alo, uint64_t ahi, int scale)
{
    int tlog2 = log2_128(alo, ahi);
    if (tlog2 < 0) tlog2 = 0;
    return tlog2 - (scale * 33219) / 10000;
}

static int decimalLog2(il2cpp_decimal_repr* pA)
{
	int tlog2;
	int scale = pA->u.signscale.scale;

	if (pA->hi32 != 0) tlog2 = 64 + log2_32(pA->hi32);
	else if (pA->mid32 != 0) tlog2 = 32 + log2_32(pA->mid32);
	else tlog2 = log2_32(pA->lo32);

	if (tlog2 != DECIMAL_LOG_NEGINF) {
		tlog2 -= (scale * 33219) / 10000;
	}

	return tlog2;
}

static int normalize128(uint64_t* pclo, uint64_t* pchi, int* pScale, int roundFlag, int roundBit)
{
	uint32_t overhang = (uint32_t)(*pchi >> 32);
	int scale = *pScale;
	int deltaScale;

	while (overhang != 0) {
		for (deltaScale = 1; deltaScale < DECIMAL_MAX_INTFACTORS; deltaScale++)
		{
			if (overhang < constantsDecadeInt32Factors[deltaScale])
				break;
		}

		scale -= deltaScale;

		if (scale < 0) 
			return DECIMAL_OVERFLOW;

		roundBit = div128by32(pclo, pchi, constantsDecadeInt32Factors[deltaScale], 0);

		overhang = (uint32_t)(*pchi >> 32);
		if (roundFlag && roundBit && *pclo == (uint64_t)-1 && (int32_t)*pchi == (int32_t)-1)
			overhang = 1;
	}

	*pScale = scale;

	if (roundFlag && roundBit)
		roundUp128(pclo, pchi);

	return DECIMAL_SUCCESS;
}

static int adjustScale128(uint64_t* palo, uint64_t* pahi, int deltaScale)
{
    int idx, rc;

    if (deltaScale < 0) {
        deltaScale *= -1;
        if (deltaScale > DECIMAL_MAX_SCALE) return DECIMAL_INTERNAL_ERROR;
        while (deltaScale > 0) {
            idx = (deltaScale > DECIMAL_MAX_INTFACTORS) ? DECIMAL_MAX_INTFACTORS : deltaScale;
            deltaScale -= idx;
            div128by32(palo, pahi, constantsDecadeInt32Factors[idx], 0);
        }
    } else if (deltaScale > 0) {
        if (deltaScale > DECIMAL_MAX_SCALE) return DECIMAL_INTERNAL_ERROR;
        while (deltaScale > 0) {
            idx = (deltaScale > DECIMAL_MAX_INTFACTORS) ? DECIMAL_MAX_INTFACTORS : deltaScale;
            deltaScale -= idx;
            rc = mult128by32(palo, pahi, constantsDecadeInt32Factors[idx], 0);
            if (rc != DECIMAL_SUCCESS) return rc;
        }
    }
    
    return DECIMAL_SUCCESS;
}

static int rescale128(uint64_t* pclo, uint64_t* pchi, int* pScale, int texp,
                                int minScale, int maxScale, int roundFlag)
{
    uint32_t factor, overhang;
    int scale, i, rc, roundBit = 0;

    scale = *pScale;

    if (texp > 0) {
        /* reduce exp */
        while (texp > 0 && scale <= maxScale) {
            overhang = (uint32_t)(*pchi >> 32);

			/* The original loop was this: */
			/*
            while (texp > 0 && (overhang > (2<<DECIMAL_MAX_INTFACTORS) || (*pclo & 1) == 0)) {
				if (--texp == 0)
					roundBit = (int)(*pclo & 1);
                rshift128(pclo, pchi);
                overhang = (guint32)(*pchi >> 32);
            }
			*/
			if (overhang > 0) {
				int msf = my_g_bit_nth_msf (overhang);
				int shift = msf - (DECIMAL_MAX_INTFACTORS + 2);

				if (shift >= texp)
					shift = texp - 1;

				if (shift > 0) {
					texp -= shift;
					*pclo = (*pclo >> shift) | ((*pchi & ((1 << shift) - 1)) << (64 - shift));
					*pchi >>= shift;
					overhang >>= shift;

					assert (texp > 0);
					assert (overhang > (2 << DECIMAL_MAX_INTFACTORS));
				}
			}
            while (texp > 0 && (overhang > (2<<DECIMAL_MAX_INTFACTORS) || (*pclo & 1) == 0)) {
				if (--texp == 0) roundBit = (int)(*pclo & 1);
                rshift128(pclo, pchi);
                overhang >>= 1;
            }

            if (texp > DECIMAL_MAX_INTFACTORS) i = DECIMAL_MAX_INTFACTORS;
            else i = texp;
            if (scale + i > maxScale) i = maxScale - scale;
            if (i == 0) break;
            texp -= i;
            scale += i;
            factor = constantsDecadeInt32Factors[i] >> i; /* 10^i/2^i=5^i */
            mult128by32(pclo, pchi, factor, 0);
    /*printf("3: %.17e\n", (((double)chi) * pow(2,64) + clo) * pow(10, -scale) * pow(2, -texp));*/
        }

        while (texp > 0) {
            if (--texp == 0) roundBit = (int)(*pclo & 1);
            rshift128(pclo, pchi);
        }
    }

    while (scale > maxScale) {
        i = scale - maxScale;
        if (i > DECIMAL_MAX_INTFACTORS) i = DECIMAL_MAX_INTFACTORS;
        scale -= i;
        roundBit = div128by32(pclo, pchi, constantsDecadeInt32Factors[i], 0);
    }

    while (scale < minScale) {
        if (!roundFlag) roundBit = 0;
        i = minScale - scale;
        if (i > DECIMAL_MAX_INTFACTORS) i = DECIMAL_MAX_INTFACTORS;
        scale += i;
        rc = mult128by32(pclo, pchi, constantsDecadeInt32Factors[i], roundBit);
        if (rc != DECIMAL_SUCCESS) return rc;
        roundBit = 0;
    }

    *pScale = scale;

    return normalize128(pclo, pchi, pScale, roundFlag, roundBit);
}

static void div128DecadeFactor(uint64_t* palo, uint64_t* pahi, int powerOfTen)
{
    int idx, roundBit = 0;

    while (powerOfTen > 0) {
        idx = (powerOfTen > DECIMAL_MAX_INTFACTORS) ? DECIMAL_MAX_INTFACTORS : powerOfTen;
        powerOfTen -= idx;
        roundBit = div128by32(palo, pahi, constantsDecadeInt32Factors[idx], 0);
    }

    if (roundBit) roundUp128(palo, pahi);
}

static void buildIEEE754Double(double* pd, int sign, int texp, uint64_t mantisse)
{
    uint64_t* p = (uint64_t*) pd;

    *p = (((uint64_t)sign) << 63) | (((uint64_t)((1023+texp)&0x7ff)) << 52) | mantisse;

	NOT_IMPLEMENTED_NO_ASSERT (buildIEEE754Double, "Endianess is not being considered");
}

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{

int Decimal::decimalSetExponent(il2cpp_decimal_repr* pA, int texp)
{
	int scale = pA->u.signscale.scale;

	scale -= texp;

	if (scale < 0 || scale > DECIMAL_MAX_SCALE)
	{
		uint64_t alo;
		uint64_t ahi;

		DECTO128(pA, alo, ahi);
		int status = rescale128(&alo, &ahi, &scale, 0, 0, DECIMAL_MAX_SCALE, 1);
		if (status != DECIMAL_SUCCESS)
			return status;

		return pack128toDecimal(pA, alo, ahi, scale, pA->u.signscale.sign);
	}
	else
	{
		pA->u.signscale.scale = scale;
		return DECIMAL_SUCCESS;
	}
}

int Decimal::string2decimal(il2cpp_decimal_repr *pA, Il2CppString *str, unsigned int decrDecimal, int sign)
{
	Il2CppChar* buf = vm::String::GetChars(str);
	Il2CppChar* p;
	uint64_t alo, ahi;
	alo = ahi = 0;
	int n, rc, i, len, sigLen = -1, firstNonZero;
	int scale, roundBit = 0;

	DECINIT(pA);

	for (p = buf, len = 0; *p != 0; len++, p++) {}

	for (p = buf, i = 0; *p != 0; i++, p++) {
		n = *p - '0';
		if (n < 0 || n > 9)
			return DECIMAL_INVALID_CHARACTER;

		if (n) {
			if (sigLen < 0) {
				firstNonZero = i;
				sigLen = (len - firstNonZero > DECIMAL_MAX_SCALE + 1) ? DECIMAL_MAX_SCALE + 1 + firstNonZero : len;
				if (decrDecimal > static_cast<uint32_t>(sigLen + 1))
					return DECIMAL_OVERFLOW;
			}
			if (i >= sigLen)
				break;
			rc = incMultConstant128(&alo, &ahi, sigLen - 1 - i, n);
			if (rc != DECIMAL_SUCCESS)
				return rc;
		}
	}

	scale = sigLen - decrDecimal;

	if (i < len) { /* too much digits, we must round */
		n = buf[i] - '0';
		if (n < 0 || n > 9) {
			return DECIMAL_INVALID_CHARACTER;
		}
		if (n > 5) roundBit = 1;
		else if (n == 5) { /* we must take a nearer look */
			n = buf[i - 1] - '0';
			for (++i; i < len; ++i) {
				if (buf[i] != '0') break; /* we are greater than .5 */
			}
			if (i < len /* greater than exactly .5 */
				|| n % 2 == 1) { /* exactly .5, use banker's rule for rounding */
				roundBit = 1;
			}
		}
	}

	if (ahi != 0) {
		rc = normalize128(&alo, &ahi, &scale, 1, roundBit);
		if (rc != DECIMAL_SUCCESS) 
			return rc;
	}

	if (alo == 0 && ahi == 0) {
		DECINIT(pA);
		return DECIMAL_SUCCESS;
	}
	else {
		return pack128toDecimal(pA, alo, ahi, sigLen - decrDecimal, sign);
	}
	return 0;
}

int Decimal::decimalCompare(il2cpp_decimal_repr *pA,il2cpp_decimal_repr *pB)
{
	int log2a, log2b, delta, sign;
	il2cpp_decimal_repr aa;

	sign = (pA->u.signscale.sign) ? -1 : 1;

	if (pA->u.signscale.sign ^ pB->u.signscale.sign) {
		return (decimalIsZero(pA) && decimalIsZero(pB)) ? 0 : sign;
	}

	/* try fast comparison via log2 */
	log2a = decimalLog2(pA);
	log2b = decimalLog2(pB);
	delta = log2a - log2b;
		/* decimalLog2 is not exact, so we can say nothing 
		if abs(delta) <= 1 */
	if (delta < -1) return -sign;
	if (delta > 1) return sign;

	DECCOPY(&aa, pA);
	DECNEGATE(&aa);
	decimalIncr(&aa, pB);

	if (decimalIsZero(&aa)) return 0;

	return (aa.u.signscale.sign) ? 1 : -1;
}

int32_t Decimal::decimalIncr (il2cpp_decimal_repr * pA, il2cpp_decimal_repr * pB)
{
	uint64_t alo, ahi, blo, bhi;
	int log2A, log2B, log2Result, log10Result, rc;
	int subFlag, sign, scaleA, scaleB;

	DECTO128(pA, alo, ahi);
	DECTO128(pB, blo, bhi);

	sign = pA->u.signscale.sign;
	subFlag = sign - (int)pB->u.signscale.sign;
	scaleA = pA->u.signscale.scale;
	scaleB = pB->u.signscale.scale;
	if (scaleA == scaleB) {
		/* same scale, that's easy */
		if (subFlag) {
			sub128(alo, ahi, blo, bhi, &alo, &ahi);
			if (ahi & LIT_GUINT64_HIGHBIT) {
				alo--;
				alo = ~alo;
				if (alo == 0) ahi--;
				ahi = ~ahi;
				sign = !sign;
			}
		} else {
			add128(alo, ahi, blo, bhi, &alo, &ahi);
		}
		rc = normalize128(&alo, &ahi, &scaleA, 1, 0);
	} else {
		/* scales must be adjusted */
		/* Estimate log10 and scale of result for adjusting scales */
		log2A = log2withScale_128(alo, ahi, scaleA);
		log2B = log2withScale_128(blo, bhi, scaleB);
		log2Result = MAX (log2A, log2B);
		if (!subFlag) log2Result++; /* result can have one bit more */
		log10Result = (log2Result * 1000) / 3322 + 1;
		/* we will calculate in 128bit, so we may need to adjust scale */
		if (scaleB > scaleA) scaleA = scaleB;
		if (scaleA + log10Result > DECIMAL_MAX_SCALE + 7) {
			/* this may not fit in 128bit, so limit it */
			scaleA = DECIMAL_MAX_SCALE + 7 - log10Result;
		}

		rc = adjustScale128(&alo, &ahi, scaleA - (int)pA->u.signscale.scale);
		if (rc != DECIMAL_SUCCESS) return rc;
		rc = adjustScale128(&blo, &bhi, scaleA - scaleB);
		if (rc != DECIMAL_SUCCESS) return rc;

		if (subFlag) {
			sub128(alo, ahi, blo, bhi, &alo, &ahi);
			if (ahi & LIT_GUINT64_HIGHBIT) {
				alo--;
				alo = ~alo;
				if (alo == 0) ahi--;
				ahi = ~ahi;
				sign = !sign;
			}
		} else {
			add128(alo, ahi, blo, bhi, &alo, &ahi);
		}

		rc = rescale128(&alo, &ahi,&scaleA, 0, 0, DECIMAL_MAX_SCALE, 1);
	}

	if (rc != DECIMAL_SUCCESS) return rc;

	return pack128toDecimal(pA, alo, ahi, scaleA, sign);
}

void Decimal::decimalFloorAndTrunc (il2cpp_decimal_repr * pA, int32_t floorFlag)
{
	uint64_t alo, ahi;
	uint32_t factor, rest;
	int scale, sign, idx;
	int hasRest = 0;

	scale = pA->u.signscale.scale;
	if (scale == 0) return; /* nothing to do */

	DECTO128(pA, alo, ahi);
	sign = pA->u.signscale.sign;

	while (scale > 0) {
		idx = (scale > DECIMAL_MAX_INTFACTORS) ? DECIMAL_MAX_INTFACTORS : scale;
		factor = constantsDecadeInt32Factors[idx];
		scale -= idx;
		div128by32(&alo, &ahi, factor, &rest);
		hasRest = hasRest || (rest != 0);
	}

	if (floorFlag && hasRest && sign) { /* floor: if negative, we must round up */
		roundUp128(&alo, &ahi);
	}

	pack128toDecimal(pA, alo, ahi, 0, sign);
}

int32_t Decimal::decimal2UInt64 (il2cpp_decimal_repr * pA, uint64_t* pResult)
{
    uint64_t alo, ahi;
    int scale;

    DECTO128(pA, alo, ahi);
    scale = pA->u.signscale.scale;
    if (scale > 0) {
        div128DecadeFactor(&alo, &ahi, scale);
    }

    /* overflow if integer too large or < 0 */
    if (ahi != 0 || (alo != 0 && pA->u.signscale.sign)) return DECIMAL_OVERFLOW;

    *pResult = alo;
    return DECIMAL_SUCCESS;
}

int32_t Decimal::decimal2Int64 (il2cpp_decimal_repr * pA, int64_t* pResult)
{
    uint64_t alo, ahi;
    int sign, scale;

    DECTO128(pA, alo, ahi);
    scale = pA->u.signscale.scale;
    if (scale > 0) {
        div128DecadeFactor(&alo, &ahi, scale);
    }

    if (ahi != 0) return DECIMAL_OVERFLOW;

    sign = pA->u.signscale.sign;
    if (sign && alo != 0) {
        if (alo > LIT_GUINT64_HIGHBIT) return DECIMAL_OVERFLOW;
        *pResult = (int64_t) ~(alo-1);
    } else {
        if (alo & LIT_GUINT64_HIGHBIT) return DECIMAL_OVERFLOW;
        *pResult = (int64_t) alo;
    }

    return DECIMAL_SUCCESS;
}

int32_t Decimal::decimalMult(il2cpp_decimal_repr* pA, il2cpp_decimal_repr* pB)
{
	uint64_t low, mid, high;
	uint32_t factor;
	int scale, sign;

	mult96by96to192(pA->lo32, pA->mid32, pA->hi32, pB->lo32, pB->mid32, pB->hi32, &low, &mid, &high);

	/* adjust scale and sign */
	scale = (int)pA->u.signscale.scale + (int)pB->u.signscale.scale;
	sign = pA->u.signscale.sign ^ pB->u.signscale.sign;

	/* first scaling step */
	factor = constantsDecadeInt32Factors[DECIMAL_MAX_INTFACTORS];
	while (high != 0 || (mid >> 32) >= factor)
	{
		if (high < 100)
		{
			factor /= 1000; /* we need some digits for final rounding */
			scale -= DECIMAL_MAX_INTFACTORS - 3;
		}
		else
		{
			scale -= DECIMAL_MAX_INTFACTORS;
		}

		div192by32(&low, &mid, &high, factor);
	}

	/* second and final scaling */
	int status = rescale128(&low, &mid, &scale, 0, 0, DECIMAL_MAX_SCALE, 1);
	if (status != DECIMAL_SUCCESS)
		return status;

	return pack128toDecimal(pA, low, mid, scale, sign);
}

int32_t Decimal::decimalDiv(il2cpp_decimal_repr* pC, il2cpp_decimal_repr* pA, il2cpp_decimal_repr* pB)
{
	uint64_t clo, chi; /* result */
	int scale, texp;

	/* Check for common cases */
	if (decimalCompare(pA, pB) == 0)
	{
		/* One */
		return pack128toDecimal(pC, 1, 0, 0, 0);
	}
	pA->u.signscale.sign = pA->u.signscale.sign ? 0 : 1;
	if (decimalCompare(pA, pB) == 0)
	{
		/* Minus one */
		return pack128toDecimal(pC, 1, 0, 0, 1);
	}
	pA->u.signscale.sign = pA->u.signscale.sign ? 0 : 1;

	int status = decimalDivSub(pA, pB, &clo, &chi, &texp);
	if (status != DECIMAL_SUCCESS)
	{
		if (status == DECIMAL_FINISHED)
			status = DECIMAL_SUCCESS;
		return status;
	}

	/* adjust scale and sign */
	scale = (int)pA->u.signscale.scale - (int)pB->u.signscale.scale;

	/*test: printf("0: %.17e\n", (((double)chi) * pow(2,64) + clo) * pow(10, -scale) * pow(2, -exp));*/
	status = rescale128(&clo, &chi, &scale, texp, 0, DECIMAL_MAX_SCALE, 1);
	if (status != DECIMAL_SUCCESS)
		return status;

	return pack128toDecimal(pC, clo, chi, scale, pA->u.signscale.sign ^ pB->u.signscale.sign);
}

double Decimal::decimal2double (il2cpp_decimal_repr * pA)
{
    double d;
    uint64_t alo, ahi, mantisse;
    uint32_t overhang, factor, roundBits;
    int scale, texp, log5, i;


    ahi = (((uint64_t)(pA->hi32)) << 32) | pA->mid32;
    alo = ((uint64_t)(pA->lo32)) << 32;

    /* special case zero */
    if (ahi == 0 && alo == 0) return 0.0;

    texp = 0;
    scale = pA->u.signscale.scale;

    /* transform n * 10^-scale and exp = 0 => m * 2^-exp and scale = 0 */
    while (scale > 0) {
        while ((ahi & LIT_GUINT64_HIGHBIT) == 0) {
            lshift128(&alo, &ahi);
            texp++;
        }

        overhang = (uint32_t) (ahi >> 32);
        if (overhang >= 5) {
            /* estimate log5 */
            log5 = (log2_32(overhang) * 1000) / 2322; /* ln(5)/ln(2) = 2.3219... */
            if (log5 < DECIMAL_MAX_INTFACTORS) {
                /* get maximal factor=5^i, so that overhang / factor >= 1 */
                factor = constantsDecadeInt32Factors[log5] >> log5; /* 5^n = 10^n/2^n */
                i = log5 + overhang / factor;
            } else {
                i = DECIMAL_MAX_INTFACTORS; /* we have only constants up to 10^DECIMAL_MAX_INTFACTORS */
            }
            if (i > scale) i = scale;
            factor = constantsDecadeInt32Factors[i] >> i; /* 5^n = 10^n/2^n */
            /* n * 10^-scale * 2^-exp => m * 10^-(scale-i) * 2^-(exp+i) with m = n * 5^-i */
            div128by32(&alo, &ahi, factor, 0);
            scale -= i;
            texp += i;
        }
    }

    /* normalize significand (highest bit should be 1) */
    while ((ahi & LIT_GUINT64_HIGHBIT) == 0) {
        lshift128(&alo, &ahi);
        texp++;
    }

    /* round to nearest even */
    roundBits = (uint32_t)ahi & 0x7ff;
    ahi += 0x400;
    if ((ahi & LIT_GUINT64_HIGHBIT) == 0) { /* overflow ? */
        ahi >>= 1;
	texp--;
    } else if ((roundBits & 0x400) == 0) ahi &= ~1;

    /* 96 bit => 1 implizit bit and 52 explicit bits */
    mantisse = (ahi & ~LIT_GUINT64_HIGHBIT) >> 11;

    buildIEEE754Double(&d, pA->u.signscale.sign, -texp+95, mantisse);

    return d;
}

int32_t Decimal::decimal2string (il2cpp_decimal_repr* val, int32_t digits, int32_t decimals, Il2CppArray* bufDigits, int32_t bufSize, int32_t* decPos, int32_t* sign)
{
	NOT_IMPLEMENTED_ICALL (Decimal::decimal2string);
	
	return 0;
}

int32_t Decimal::decimalIntDiv (il2cpp_decimal_repr* pC, il2cpp_decimal_repr* pA, il2cpp_decimal_repr* pB)
{
	uint64_t clo, chi; /* result */
	int scale, texp, rc;

	rc = decimalDivSub (pA, pB, &clo, &chi, &texp);
	if ( rc != DECIMAL_SUCCESS ) {
		if ( rc == DECIMAL_FINISHED ) rc = DECIMAL_SUCCESS;
		return rc;
	}

	/* calc scale  */
	scale = (int)pA->u.signscale.scale - (int)pB->u.signscale.scale;

	/* truncate result to integer */
	rc = rescale128 (&clo, &chi, &scale, texp, 0, 0, 0);
	if ( rc != DECIMAL_SUCCESS ) return rc;

	return pack128toDecimal (pC, clo, chi, scale, pA->u.signscale.sign);
}

} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
