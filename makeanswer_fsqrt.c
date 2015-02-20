/*
～使い方～
使用する関数に応じて、11行目、60行目を書き換える。
あらかじめtestcase.txtは準備しておく必要がある。

finv.c, fsqrt.cの場合、-lmが必要となるので注意。
(例)gcc finv.c -lm
*/


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "fsqrt.c"                  //ここを書き換える
#include "def.c"

#define INPUT  "testcase-mono.txt"
#define OUTPUT "answer.txt"

//2^32-1まで。32bitで返す
void output_32bit(uint32_t num, FILE *fp) {
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

uint32_t to_uint32(char *text) {
  int i;
  uint32_t num = 0;
  for (i = 0; i < 32; i++) {
    if (text[31-i] == '1') {
      num += (1 << i);
    }
  }
  return (num);
}

int main(void) {

  FILE *input, *output;
  char a_text[33], b_text[33];
  uint32_t a, ans;

  if ((input = fopen(INPUT, "r")) == NULL) {
    printf("file open error.\n");
    exit(EXIT_FAILURE);
  }
  if ((output = fopen(OUTPUT, "w")) == NULL) {
    printf("file open error.\n");
    exit(EXIT_FAILURE);
  }
  
  while (fscanf(input, "%s", a_text) != EOF) {
    a = to_uint32(a_text);
    ans = fsqrt(a);               //ここを書き換える
    output_32bit(ans, output);
  }
  
  close(input);
  close(output);

  printf("success (> %s)\n", OUTPUT);

  return 0;
}
