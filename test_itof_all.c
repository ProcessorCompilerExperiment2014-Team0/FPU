/*
  コンパイル時コマンド
  gcc test_itof_all.c itof.c print.c def.c
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "def.h"
#include "print.h"

#define PERMIT 1  //誤差の許容範囲　何ulpまでか

uint32_t itof(uint32_t org);

uint32_t str_to_uint32t(char *str) {
  int i;
  uint32_t result = 0;
  
  for (i = 0; i < 32; i++) {
    if (str[i] == '1') {
      result += (1 << (31 - i));
    }
  }
  //printf("%u\n", result); //debug
  return (result);
}

int to_signed_int(union data_32bit a) {
  int i;
  int flag;
  int ans;
  flag = a.uint32 >> 31;
  if (flag == 0) {
    ans = a.uint32;
  } else {
    ans = ~(a.uint32 - 1);
    ans = ans * (-1);
  }
  return ans;
}

void show_testcase(union data_32bit a,
		   union data_32bit result, union data_32bit correct) {
  printf("-- a --\n");
  print_data(a);
  putchar('\n');
  printf("-- result --\n");
  print_data(result);
  putchar('\n');
  printf("-- correct --\n");
  print_data(correct);
  putchar('\n');
  printf("-------------------------------------------------\n");
}

//非正規化数を全て０にする
uint32_t normalize(uint32_t a) {
  union data_32bit temp;
  temp.uint32 = a;
  if (temp.exp == 0) {
    temp.frac = 0;
  }
  return (temp.uint32);
}


int count_diff(uint32_t a_uint32, uint32_t b_uint32) {
  union data_32bit a, b;
  int diff;
  a.uint32 = a_uint32;
  b.uint32 = b_uint32;
  if ((a.exp == 255) && (a.frac != 0)) { // aがNaNの場合
    if ((b.exp == 255) && (b.frac != 0)) {
      return 0; //NaNは全て同一視
    } else {
      return (PERMIT + 1);
    }
  } else if (a.exp == 0) {
    if (b.exp == 0) {
      if (a.sign == b.sign) {
	return 0;
      } else {
	return (PERMIT + 1);
      }
    } else {
      return (PERMIT + 1);
    }
  } else {
    if (a.uint32 == b.uint32) {
      return 0;
    } else {
      diff = a.uint32 - b.uint32;
      if (diff >= 0) {
	return (diff);
      } else {
	return ((-1) * diff);
      }
    }
  }
}

int main(void) {
  union data_32bit a, result, correct;
  uint32_t i;
  long unsigned int total_mistakes  = 0;   //誤答数をカウント
  long unsigned int count_total_diff[PERMIT+1];
  int count, j;
  
  for (i = 0; i < PERMIT+1; i++) {
    count_total_diff[i] = 0;
  }
  
  for (i = 0; i < 4294967295; i++) {
    
    if ((i % 100000000) == 0) {
      printf("> checked (%2u/42)\n", i / 100000000);
    }
    
    a.uint32 = i;
    //n.uint32 = normalize(i);    //もともとintなので不要
    result.uint32 = itof(a.uint32);
    correct.fl32 = (float)to_signed_int(a);
    
    count = count_diff(result.uint32, correct.uint32);
    if ((-1 * PERMIT - 1) < count && count < PERMIT+1) {
      count_total_diff[count]++;
    } else {
      if (total_mistakes <= 5) {
	show_testcase(a, result, correct);
      }
      total_mistakes++;
    }
  }
  
  for (j = 0; j < PERMIT+1; j++) {
    printf("total %dulp diff : %lu\n", j, count_total_diff[j]);
  }
  printf("total mistakes   : %lu\n", total_mistakes);
  
  return 0;
}
