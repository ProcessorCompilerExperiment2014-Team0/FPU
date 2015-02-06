/* コンパイル時　gcc maketable_fsqrt_c.c -lm */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX 512
#define FRAC_MAX 8388607
#define MASK13  8191
#define OUTPUT "fsqrt_table.dat"

union data_32bit {
  struct {
    unsigned int frac : 23;
    unsigned int exp  : 8;
    unsigned int sign : 1;
  };
  float fl32;
  uint32_t uint32;
};

double make_a(double t, double c) {
  double a;
  a = 1 / (sqrt(t+c) + sqrt(t));
  return a;
}

double make_b(double t, double c) {
  double b, temp;
  temp = (2 * make_a(t,c) * sqrt(t)) - 1;
  b = (2 - temp * temp) / (8 * make_a(t,c));
  return b;
}

double make_y(double a, double b, double x) {
  double y;
  y = a * x + b;
  return y;
}

int main(void) {
  int i;
  double x_range;
  double c;  // あとで定義
  double t, a_db, b_db, y1_db, y2_db;
  uint32_t y1_mant, y2_mant, d, exception;
  unsigned long int rom_data;
  FILE *fp;
  union data_32bit y1,y2;
  
  if ((fp = fopen(OUTPUT, "w")) == NULL) {
    printf("file open error. (%s)\n", OUTPUT);
    exit(EXIT_FAILURE);
  }
  
  // loop (1 ~ 2)
  x_range = 1.0;
  c = x_range / MAX;
  for (i = 0; i < MAX; i++) {
    t = x_range + c * i;
    a_db = make_a(t,c);
    b_db = make_b(t,c);
    y1_db = make_y(a_db, b_db, t);
    y2_db = make_y(a_db, b_db, t+c);
    y1.fl32 = (float)y1_db;
    y2.fl32 = (float)y2_db;

    y1_mant = (1 << 23) + y1.frac;
    y2_mant = (1 << 23) + y2.frac;

    d = (y2_mant - y1_mant) & MASK13;
    
    rom_data = ((unsigned long int)y1.frac << 13) + d;
  
    fprintf(fp, "%09lx\n", rom_data);
  }


  // loop (2 ~ 4)
  x_range = 2.0;
  c = x_range / MAX;
  for (i = 0; i < MAX; i++) {
    t = x_range + c * i;
    a_db = make_a(t,c);
    b_db = make_b(t,c);
    y1_db = make_y(a_db, b_db, t);
    y2_db = make_y(a_db, b_db, t+c);
    y1.fl32 = (float)y1_db;
    y2.fl32 = (float)y2_db;
    
    y1_mant = (1 << 23) + y1.frac;
    if (i != (MAX - 1)) {
      y2_mant = (1 << 23) + y2.frac;
    } else {
      y2_mant = 1 << 24;
    }
    d = (y2_mant - y1_mant) & MASK13;
    
    rom_data = ((unsigned long int)y1.frac << 13) + d;
    
    fprintf(fp, "%09lx\n", rom_data);
  }

  
  close(fp);
  
  printf("success (> %s)\n", OUTPUT);
  
  return 0;
}
      
      



