#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "def.h"
#include "print.h"

uint32_t fsqrt(uint32_t a);

int main(void) {
  
  union data_32bit a;
  union data_32bit result;

  int select_flag;
  char select_buf[10];
  printf("select import form :\n");
  printf("float -> 0\n");
  printf("32bit -> 1 (others)\n");
  gets(select_buf);
  sscanf(select_buf, "%d\n", &select_flag);

  if (select_flag == 0) {
    printf("a.fl32 : "); scanf("%f", &a.fl32);
  } else {
    char a_str[35];
    printf("詰めて入力しても可\n");
    printf("- --exp--- -------fraction--------\n");
    printf("a(32bit) :\n");
    gets(a_str);
    a.uint32 = str_to_uint32(delete_space(a_str));
  }
    
  printf("\n");

  printf("-- a --\n");
  print_data(a);
  
  printf("\n");

  result.uint32 = fsqrt(a.uint32);
  
  printf("-- fsqrt(result) --\n");
  print_data(result);

  printf("\n");

  union data_32bit test;

  printf("-- correct answer --\n");
  test.fl32 = sqrtf(a.fl32);
  print_data(test);

  return(0);
}
