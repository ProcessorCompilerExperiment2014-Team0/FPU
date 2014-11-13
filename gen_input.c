#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include "MT.h"
#include "def.h"

const int times = 1<<13;

uint32_t
rand32()
{
//	return *(uint32_t *)&genrand_int32;
	uint32_t x;
	x = rand() & 255;
	x = (rand() & 255) + (x<<8);
	x = (rand() & 255) + (x<<8);
	x = (rand() & 255) + (x<<8);
	return x;
}


void
gen_fcmp_gt()
{
	union data_32bit a, b, s;

	for (int i=0; i < times; i++)
	{
		a.uint32 = rand32();
		b.uint32 = rand32();
		s.uint32 = (a.fl32 > b.fl32)?1:0;

		printf("%08X %08X %08X\n", a.uint32, b.uint32, s.uint32);
	}
}

void
gen_fcmp_eq()
{
	union data_32bit a, b, s;

	for (int i=0; i < times; i++)
	{
		a.uint32 = rand32();
		b.uint32 = rand32();
		s.uint32 = (a.fl32 == b.fl32)?1:0;

		printf("%08X %08X %08X\n", a.uint32, b.uint32, s.uint32);
	}

	for (int i=0; i < times; i++)
	{
		a.uint32 = rand32();
		s.uint32 = (a.fl32 == a.fl32)?1:0;

		printf("%08X %08X %08X\n", a.uint32, a.uint32, s.uint32);
	}
}

uint32_t itof(uint32_t a);

void
gen_itof()
{
	union data_32bit a, s;

	for (int i=0; i < times; i++)
	{
		a.uint32 = rand32();
		s.uint32 = itof(a.uint32);

		printf("%08X %08X\n", a.uint32, s.uint32);
	}
}

uint32_t ftrc(uint32_t a);

void
gen_ftoi()
{
	union data_32bit a, s;

	for (int i=0; i < times; i++)
	{
		a.uint32 = rand32();
		s.uint32 = ftrc(a.uint32);

		printf("%08X %08X\n", a.uint32, s.uint32);
	}
}



int
main(int argc, char** argv)
{
	srand((unsigned) time(NULL));
	init_genrand((unsigned) time(NULL));

	if (argc != 2) {
		printf("gen_input (fcmp_gt|fcmp_eq|ftoi|itof)\n");
		return 0;
	}

	if (strcmp(argv[1], "fcmp_gt_tb") == 0)
		gen_fcmp_gt();
	else if (strcmp(argv[1], "fcmp_eq_tb") == 0)
		gen_fcmp_eq();
	else if (strcmp(argv[1], "itof_tb") == 0)
		gen_itof();
	else if (strcmp(argv[1], "ftoi_tb") == 0)
		gen_ftoi();
	else
		printf("gen_input (fcmp_gt|fcmp_eq|ftoi|itof)\n");

	return 0;
}
