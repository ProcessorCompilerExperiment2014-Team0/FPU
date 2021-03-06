#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "def.h"
#include "print.h"

uint32_t fmul(uint32_t a, uint32_t b);

int main(void) {
  
  union data_32bit a,b;
  union data_32bit sum;

  int select_flag;
  char select_buf[10];
  printf("select import form :\n");
  printf("float -> 0\n");
  printf("32bit -> 1 (others)\n");
  gets(select_buf);
  sscanf(select_buf, "%d\n", &select_flag);

  if (select_flag == 0) {
    printf("a.fl32 : "); scanf("%f", &a.fl32);
    printf("b.fl32 : "); scanf("%f", &b.fl32);
  } else {
    char a_str[35], b_str[35];
    printf("詰めて入力しても可\n");
    printf("- --exp--- -------fraction--------\n");
    printf("a(32bit) :\n");
    gets(a_str);
    printf("b(32bit) :\n");
    gets(b_str);
    a.uint32 = str_to_uint32(delete_space(a_str));
    b.uint32 = str_to_uint32(delete_space(b_str));
  }
    
  printf("\n");

  printf("-- a --\n");
  print_data(a);
  
  printf("\n");

  printf("-- b --\n");
  print_data(b);

  printf("\n");

  sum.uint32 = fmul(a.uint32, b.uint32);
  
  printf("-- product --\n");
  print_data(sum);

  printf("\n");

  union data_32bit test;

  printf(" -- correct answer --\n");
  test.fl32 = a.fl32 * b.fl32;
  print_data(test);

  return(0);
}
