EXES = fadd fmul fneg fsub finv fsqrt
TESTS = test_fadd test_fmul test_finv test_fsqrt
LIBS = def.o print.o
CC = gcc
CFLAGS = -std=c99 -O2 -Wall
LD = gcc
LDFLAGS = -lm

TESTBENCH = fcmp_eq_tb fcmp_gt_tb ftoi_tb itof_tb
SOURCES =  fcmp.vhd fcmp_eq_tb.vhd fcmp_gt_tb.vhd ftoi_tb.vhd ftoi_func.vhd itof_tb.vhd itof_func.vhd fpu_common.vhd
GHDLC = ghdl
GHDLFLAGS  = -g --ieee=synopsys --mb-comments -fexplicit
GHDL_SIM_OPT = --stop-time=20ms

all: $(EXES) $(TESTS)

fadd: fadd_main.o fadd.o $(LIBS)

fmul: fmul_main.o fmul.o $(LIBS)

fneg: fneg_main.o fneg.o $(LIBS)

fsub: fsub_main.o fsub.o fadd.o fneg.o $(LIBS)

finv: finv_main.o finv.o fadd.o fmul.o $(LIBS)
	$(LD) -o $@ $^ $(LDFLAGS)

fsqrt: fsqrt_main.o fsqrt.o fadd.o fmul.o $(LIBS)
	$(LD) -o $@ $^ $(LDFLAGS)

test_fadd: test_fadd.o fadd.o $(LIBS)
	$(LD) -o $@ $^ $(LDFLAGS)

test_fmul: test_fmul.o fmul.o $(LIBS)
	$(LD) -o $@ $^ $(LDFLAGS)

test_finv: test_finv.o finv.o fadd.o fmul.o $(LIBS)
	$(LD) -o $@ $^ $(LDFLAGS)

test_fsqrt: test_fsqrt.o fsqrt.o fadd.o fmul.o $(LIBS)
	$(LD) -o $@ $^ $(LDFLAGS)

finv_table.dat: maketable_finv
	./maketable_finv

fsqrt_table.dat: maketable_fsqrt
	./maketable_fsqrt

table.vhd: finv_table.dat fsqrt_table.dat
	ruby table.erb

test_finv_all: finv_table.dat test_finv_all.o finv.o $(LIBS)
	$(LD) -o $@ $(filter %.o, $^) $(LDFLAGS)

clean:
	rm -f $(EXES) $(TESTS) *.o *~ work-obj93.cf

.PHONY: all clean $(TESTBENCH)

gen_input: ftrc.o itof.o gen_input.o def.o

work-obj93.cf:
	$(GHDLC) -i $(GHDLFLAGS) $(SOURCES)

$(TESTBENCH): work-obj93.cf gen_input
	$(GHDLC) -m $(GHDLFLAGS) $@
	./gen_input $@ | $(GHDLC) -r $(GHDLFLAGS) $@ $(GHDL_SIM_OPT) --wave=$@.ghw
