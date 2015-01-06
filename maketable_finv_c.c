/* コンパイル時　gcc maketable_finv.c -lm */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX 2048
#define X_RANGE 1.0
#define OUTPUT "table_finv_c.txt"

#define OVER 8192  // 2^13
//#define OVER 16384
//#define OVER 32768

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
  a = 2 / (t * (t+c));
  return a;
}

double make_b(double t, double c) {
  double b, temp;
  temp = sqrt(1/t) + sqrt(1/(t+c));
  b = temp * temp;
  return b;
}

double make_y(double a, double b, double x) {
  double y;
  y = ((-1) * a * x) + b;
  return y;
}


int main(void) {
  int i;
  double c = X_RANGE / MAX;
  double t,a_db,b_db,y1_db,y2_db;
  uint32_t y1_mant, y2_mant, d, exception;
  unsigned long int rom_data;
  FILE *fp;
  union data_32bit y1,y2;

  printf("c = %f\n", c);
  
  if ((fp = fopen(OUTPUT, "w")) == NULL) {
    printf("file open error. (%s)\n", OUTPUT);
    exit(EXIT_FAILURE);
  }
  
  for (i = 0; i < MAX; i++) {
    t = X_RANGE + c * i;
    a_db = make_a(t,c);
    b_db = make_b(t,c);
    y1_db = make_y(a_db, b_db, t);
    y2_db = make_y(a_db, b_db, t+c);
    y1.fl32 = (float)y1_db;
    y2.fl32 = (float)y2_db;

    if (i != 0) {
      y1_mant = (1 << 23) + y1.frac;
    } else {
      y1_mant = 1 << 24;
    }
    y2_mant = (1 << 23) + y2.frac;
    d = y1_mant - y2_mant;
    
    rom_data = ((unsigned long int)y2.frac << 13) + d;

    fprintf(fp, "%09lx\n", rom_data);
  }

  close(fp);
  
  printf("success (> %s)\n", OUTPUT);
  
  return 0;
}
