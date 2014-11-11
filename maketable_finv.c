/* コンパイル時　gcc maketable_finv.c -lm */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX 2048
#define X_RANGE 0.5
#define OUTPUT "table_finv.txt"

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
  a = 1 / (t * (t+c));
  return a;
}

double make_b(double t, double c) {
  double b, temp;
  temp = sqrt(1/t) + sqrt(1/(t+c));
  b = (temp * temp) / 2;
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
  FILE *fp;
  union data_32bit y1,y2;
  
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
    
    fprintf(fp, "0x%06x 0x%06x\n", y2_mant, d);
  }

  close(fp);
  
  printf("success (> %s)\n", OUTPUT);
  
  return 0;
}
