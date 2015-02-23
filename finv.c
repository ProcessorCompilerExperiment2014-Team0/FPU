/* コンパイル時　gcc finv.c -lm */

#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "def.h"

#define MASK10 1023  //((1 << 11) - 1) << 12  MAX = 2048
#define MASK13 8191     //仮数部下位13bit

uint32_t finv(uint32_t a_uint32) {
  
  union data_32bit a, result;

  a.uint32 = a_uint32;

  if (a.exp == 255 && a.frac != 0) {  // NaN
    result.uint32 = MY_NAN;
  } else if (a.exp == 0 && a.frac != 0) {// 非正規化数
    if (a.sign == 0) {
      result.uint32 = INF;
    } else {
      result.uint32 = NINF;
    }
  } else if (a.uint32 == INF) {
    result.uint32 = ZERO;
  } else if (a.uint32 == NINF) {
    result.uint32 = NZERO;
  } else if (a.uint32 == ZERO) {
    result.uint32 = INF;
  } else if (a.uint32 == NZERO) {
    result.uint32 = NINF;
  } else if (a.frac == 0) {
    int diff;
    result.uint32 = a.uint32;
    if (a.exp >= 127) {
      diff = a.exp - 127;
      result.exp = 127 - diff;
    } else {
      diff = 127 - a.exp;
      result.exp = 127 + diff;
    }
  } else if (a.exp == 254) {
    if (a.sign == 0) {
      result.uint32 = ZERO;
    } else {
      result.uint32 = NZERO;
    }
  } else {
    
    int index;
    uint64_t l;
    unsigned int y,d;
    index = (a.frac >> 13) & MASK10;
    l = finv_table[index];
    y = l >> 13;
    d = l & 0x1fff;

    result.sign = a.sign;
    result.exp  = 253 - a.exp;
    result.frac = y + ((d * (8192 - (a.frac & MASK13)) + 1) >> 12);
  }
  return (result.uint32);
}
