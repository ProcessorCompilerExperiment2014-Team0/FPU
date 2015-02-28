#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "def.h"

uint32_t
expfrac(union data_32bit a)
{
    return (a.exp << 23) + a.frac;
}

int
leading_zero_25(uint32_t a)
{
    int i;
    for (i=24; i >= 0; i--)
        if (((a >> i) & 1) != 0)
            break;

    return 24-i;
}

uint32_t
fadd(uint32_t a, uint32_t b)
{
    const uint32_t B22TO0 = (1<<23)-1;

    union data_32bit fa, fb, fc;
    union data_32bit fbig, fsmall;
    int lzc;
    int expdiff;
    int bigfrac, smallfrac;
    int rawfrac;

    fa.uint32 = a;
    fb.uint32 = b;

    if ((fa.sign == 1 && fb.sign == 1)
        || (fa.sign == 1 && expfrac(fa) > expfrac(fb))
        || (fb.sign == 1 && expfrac(fa) < expfrac(fb))) {
        fc.sign = 1;
    } else {
        fc.sign = 0;
    }

    if (fa.exp > fb.exp) {
        fbig   = fa;
        fsmall = fb;
    } else {
        fbig   = fb;
        fsmall = fa;
    }

    expdiff = fbig.exp - fsmall.exp;
    bigfrac = (1<<23)+fbig.frac;
    if (expdiff < 24) {
        smallfrac = ((1 << 23) + fsmall.frac) >> expdiff;
    } else {
        smallfrac = 0;
    }

    if (fa.sign != fb.sign && smallfrac > bigfrac) {
        rawfrac = smallfrac - bigfrac;
    } else if (fa.sign != fb.sign && bigfrac >= smallfrac)  {
        rawfrac = bigfrac - smallfrac;
    } else {
        rawfrac = bigfrac + smallfrac;
    }

    lzc = leading_zero_25(rawfrac);

    if (lzc == 0 && fbig.exp == 254) {
        fc.exp  = 255;
        fc.frac = 0;
    } else if (lzc == 0) {
        fc.exp  = fbig.exp + 1;
        fc.frac = (rawfrac >> 1) & B22TO0;
    } else if (lzc == 25 || fbig.exp < lzc) {
        fc.exp  = 0;
        fc.frac = 0;
    } else {
        fc.exp  = fbig.exp - (lzc - 1);
        fc.frac = (rawfrac << (lzc - 1)) & B22TO0;
    }

    if (fa.exp == 0) {
        return b;
    } else if (fb.exp == 0) {
        return a;
    } else if (fa.exp == 255 && fa.frac != 0) {
        return MY_NAN;
    } else if (fb.exp == 255 && fb.frac != 0) {
        return MY_NAN;
    } else if (fa.exp == 255) {
        if (fb.exp == 255 && fa.sign != fb.sign) {
            return MY_NAN;
        } else {
            return a;
        }
    } else if (fb.exp == 255) {
        return b;
    } else {
        return fc.uint32;
    }
}
