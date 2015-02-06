#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "def.h"

#define MAX    512      // 1~2,2~4をそれぞれ512分割、計1024分割
#define MASK9  8372224  // ((1 << 9) - 1) << 14 
#define MASK14 16383    // 11111111111111

static bool fsqrt_table_initalized = false;
static uint64_t fsqrt_table[1024];

void initalize_fsqrt_table() {
  FILE *f = fopen("fsqrt_table.dat", "r");
  int i;

  for (i = 0; i < 1024; i++) {
    fscanf(f, "%" SCNx64 "X\n", &fsqrt_table[i]);
  }
  
  fclose(f);
}

uint32_t fsqrt(uint32_t a_uint32) {
  
  union data_32bit a, x, result;
 
  if (!fsqrt_table_initalized) {
    fsqrt_table_initalized = true;
    initalize_fsqrt_table();
  }
 
  a.uint32 = a_uint32;

  if (a.sign == 1) {
    if (a.exp == 0) {
      result.uint32 = NZERO;
    } else {
      result.uint32 = NNAN;
    }
  } else if (a.exp == 0) {
    result.uint32 = ZERO;
  } else if (a.exp == 255 && a.frac != 0) {
    result.uint32 = MY_NAN;
  } else if (a.uint32 == INF) {
    result.uint32 = INF;
  } else {
    
    int index;
    unsigned int exp, y, d, n;
    long long unsigned int l;

    result.sign = 0;
    x.uint32 = a.uint32;

    index = (x.frac & MASK9) >> 14;
    
    if ((x.exp & 1) == 0) {      // 2の奇数乗の場合 ※exp-127
      index += (1 << 9);
    } else {
      // 何もしない
    }
    
    if (a.exp >= 127) {
      exp = a.exp - 127;
      exp = exp >> 1;
      result.exp = 127 + exp;
    } else {
      exp = 127 - a.exp;
      exp = (exp + 1) >> 1;
      result.exp = 127 - exp;
    }   

    l = fsqrt_table[index];
    y = l >> 13;
    if ((x.exp & 1) == 0) {
      d = (1 << 13) + (l & 0x1fff);
    } else {
      d = l & 0x1fff;
    }
    n = a.frac & MASK14;

    result.frac = y + ((d * n) >> 14);

  }
  return (result.uint32);
}
