/* 例外処理と仮数部全パターンをチェック */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "MT.h"

#define MAX 4294967295 // 2^32-1
#define FILE_NAME "testcase.txt"

#define ZERO     0u
#define NZERO    2147483648u
#define INF      2139095040u
#define NINF     4286578688u

union data_32bit {
  struct {
    unsigned int frac : 23;
    unsigned int exp  : 8;
    unsigned int sign : 1;
  };
  float fl32;
  uint32_t uint32;
};

//NaNを生成
uint32_t make_nan() {
  union data_32bit temp;
  temp.uint32 = genrand_int32();
  temp.exp = 255; //指数部を255に書き換え
  return (temp.uint32);
}

//非正規化数を生成
uint32_t make_denormal() {
  union data_32bit temp;
  temp.uint32 = genrand_int32();
  temp.exp = 0; //指数部を0に書き換え
  return (temp.uint32);
}

void output(uint32_t num, FILE *fp) {
  int i;
  char *temp = calloc(33, sizeof(char));
  for (i = 0; i < 32; i++) {
    if (num % 2 == 1)
      temp[31-i] = '1';
    else
      temp[31-i] = '0';
    num = num / 2;
  }
  temp[32] = '\0';
  fprintf(fp, "%s\n", temp);
  free(temp);
}

/*
void output(uint32_t num, FILE *fp) {
  fprintf(fp, "%08x\n", num);
}
*/


int main(void) {
  int i;
  uint32_t a;
  FILE *fp;

  if ((fp = fopen(FILE_NAME, "w")) == NULL) {
    printf("file open error.\n");
    exit(EXIT_FAILURE);
  }

  a = ZERO;   output(a,fp);
  a = NZERO;  output(a,fp);
  a = INF;    output(a,fp);
  a = NINF;   output(a,fp);
  
  for (i = 0; i < 1000; i++) {
    a = make_nan();
    output(a,fp);
  }
  for (i = 0; i < 1000; i++) {
    a = make_denormal();
    output(a,fp);
  }  
  for (i = 1065353216; i < 1073741825; i++) {
    output(i,fp);
  }
  
  printf("success (> %s)\n", FILE_NAME);
  
  return 0;
}
