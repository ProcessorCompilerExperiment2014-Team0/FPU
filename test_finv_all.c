/*
  コンパイル時コマンド
  gcc test_finv_all.c finv.c print.c def.c -lm
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "def.h"
#include "print.h"

#define PERMIT 10  //誤差の許容範囲　何ulpまでか

uint32_t finv(uint32_t org);

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

int main(void)
{
  union data_32bit a, n, result, correct;
  uint32_t i;
  long unsigned int total_mistakes  = 0;   //誤答数をカウント
  /*
  long unsigned int count_no_diff   = 0;
  long unsigned int count_1ulp_diff = 0;   //1bitずれをカウント
  long unsigned int count_2ulp_diff = 0;
  long unsigned int count_3ulp_diff = 0;
  */
  long unsigned int count_total_diff[PERMIT+1];
  int count, j;
  
  for (i = 0; i < PERMIT+1; i++) {
    count_total_diff[i] = 0;
  }

  for (i = 0x40000000; i < 0x42000000; i++) {

    if ((i % 100000000) == 0) {
      printf("> checked (%2u/42)\n", i / 100000000);
    }

    a.uint32 = i;
    n.uint32 = normalize(i);
    result.uint32 = finv(a.uint32);
    correct.fl32 = 1.0 / n.fl32;  //非正規仮数を0に潰す

    count = count_diff(result.uint32, correct.uint32);
    if (count < PERMIT+1) {
      count_total_diff[count]++;
    } else {
      if (total_mistakes <= 5) {
	show_testcase(a, result, correct);
      }
      total_mistakes++;
    }

    /*
    switch (count_diff(result.uint32, correct.uint32)) {
    case 0:
      count_no_diff++;   break;
    case 1:
      count_1ulp_diff++; break;
    case 2:
      count_2ulp_diff++; break;
    case 3:
      count_3ulp_diff++; break;
    default:
      total_mistakes++;  
      if (total_mistakes <= 5) {
	show_testcase(a, result, correct);
      }
      if (total_mistakes == 5) {
	printf("more than 5 mistakes.\n");
      }
      break;
    }
    */
  }
  /*
  printf("total   no diff : %lu\n", count_no_diff);
  printf("total 1ulp diff : %lu\n", count_1ulp_diff);
  printf("total 2ulp diff : %lu\n", count_2ulp_diff);
  printf("total 3ulp diff : %lu\n", count_3ulp_diff);
  printf("total mistakes  : %lu (more than 3ulp)\n", total_mistakes);
  */

  for (j = 0; j < PERMIT+1; j++) {
    printf("total %dulp diff : %lu\n", j, count_total_diff[j]);
  }
  printf("total mistakes   : %lu\n", total_mistakes);

  return 0;
}
