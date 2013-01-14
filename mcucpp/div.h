
#pragma once

#include <nlz.h>
#include <stdint.h>

///////////////////////////////////////////////////////////////////////////////
/// div64x32u performs 64bit / 32bit unsigned division givving 32 bit result.
/// Returns uint32_t maximum value if division overflows.
/// Origin:
/// http://www.hackersdelight.org/hdcodetxt/divlu.c.txt
///////////////////////////////////////////////////////////////////////////////

static inline uint32_t div64x32u(uint64_t u, uint32_t v, uint32_t *r)
{
	const uint32_t b = 65536; // Number base (16 bits).
	uint32_t un1, un0,        // Norm. dividend LSD's.
			vn1, vn0,         // Norm. divisor digits.
			q1, q0,           // Quotient digits.
			un32, un21, un10, // Dividend digit pairs.
			rhat;             // A remainder.
	int s;                    // Shift amount for norm.

	if ((uint32_t)(u >> 32) >= v)
	{
		if (r != NULL)        // to an impossible value,
			*r = 0xFFFFFFFF;  // and return the largest
		return 0xFFFFFFFF;    // possible quotient.
	}

	s = nlz(v);               // 0 <= s <= 31.
	v = v << s;               // Normalize divisor.
	vn1 = v >> 16;            // Break divisor up into
	vn0 = v & 0xFFFF;         // two 16-bit digits.

	un32 = (uint32_t)(u >> (32-s));
	un10 = (uint32_t)(u << s);// Shift dividend left.

	un1 = un10 >> 16;         // Break right half of
	un0 = un10 & 0xFFFF;      // dividend into two digits.

	q1 = un32/vn1;            // Compute the first
	rhat = un32 - q1*vn1;     // quotient digit, q1.
	again1:
	if (q1 >= b || q1*vn0 > b*rhat + un1)
	{
		q1 = q1 - 1;
		rhat = rhat + vn1;
		if (rhat < b) 
			goto again1;
	}

	un21 = un32*b + un1 - q1*v;// Multiply and subtract.
	q0 = un21/vn1;             // Compute the second
	rhat = un21 - q0*vn1;      // quotient digit, q0.
	again2:
	if (q0 >= b || q0*vn0 > b*rhat + un0)
	{
		q0 = q0 - 1;
		rhat = rhat + vn1;
		if (rhat < b) goto again2;
	}

	if (r != 0)                 // If remainder is wanted,
		*r = (un21*b + un0 - q0*v) >> s;     // return it.

	return q1*b + q0;
}

///////////////////////////////////////////////////////////////////////////////
/// div64x32s performs 64bit / 32bit signed division givving 32 bit result.
/// Returns int32_t maximum value if division overflows.
/// Origin:
/// http://www.hackersdelight.org/hdcodetxt/divlu.c.txt
///////////////////////////////////////////////////////////////////////////////

static inline int32_t div64x32s(int64_t u, int32_t v, int32_t *r)
{
	int32_t uneg = u >> 63;          // -1 if u < 0.
	if (uneg) // Compute the absolute
	{              
		u = -u;
	}

	int32_t vneg = v >> 31;           // -1 if v < 0.
	v = (v ^ vneg) - vneg;    // Absolute value of v.

	int32_t q, diff;
	if ((uint32_t)(u >> 32) >= (uint32_t)v) 
		goto overflow;

	q = div64x32u((uint64_t)u, (uint32_t)v, (uint32_t *)r);

	diff = uneg ^ vneg;       // Negate q if signs of
	q = (q ^ diff) - diff;    // u and v differed.
	if (uneg && r != 0)
		*r = -*r;

	if ((diff ^ q) < 0 && q != 0)// If overflow,
	{  
		overflow:                    // set remainder
		if (r != 0)         // to an impossible value,
			*r = 0x80000000;    // and return the largest
		q = 0x80000000;
	}       // possible neg. quotient.
	return q;
}

