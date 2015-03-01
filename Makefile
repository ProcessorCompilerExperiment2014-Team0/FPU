EXES = fadd fmul fneg fsub finv fsqrt
TESTS = test_fadd test_fmul test_finv test_fsqrt
LIBS = def.o print.o
TXTS = answer.txt result.txt testcase.txt testcase-mono.txt
CC = gcc
CFLAGS = -std=c99 -O2 -Wall
LD = gcc
LDFLAGS = -lm

TESTBENCH = fcmp_gt_tb ftoi_tb itof_tb
SOURCES =  fcmp.vhd fcmp_gt_tb.vhd ftoi_tb.vhd ftoi_func.vhd itof_tb.vhd itof_func.vhd fpu_common.vhd \
  fsqrt_tb.vhd fsqrt.vhd table.vhd fadd_pipeline.vhd fadd_tb.vhd finv.vhd finv_tb.vhd
GHDLC = ghdl
GHDLFLAGS  = -g --ieee=synopsys --mb-comments -fexplicit
GHDL_SIM_OPT = --stop-time=20ms

all: $(EXES) $(TESTS)

fadd: fadd_main.o fadd.o $(LIBS)

fmul: fmul_main.o fmul.o $(LIBS)

fneg: fneg_main.o fneg.o $(LIBS)

finv: finv_main.o finv.o fadd.o fmul.o $(LIBS)
	$(LD) -o $@ $^ $(LDFLAGS)

fsqrt: fsqrt_main.o fsqrt.o fadd.o fmul.o $(LIBS)
	$(LD) -o $@ $^ $(LDFLAGS)

Verify: Verify.o fadd.o $(LIBS)
	$(LD) -o $@ $^ $(LDFLAGS)

test_fadd: test_fadd.o fadd.o $(LIBS)
	$(LD) -o $@ $^ $(LDFLAGS)

test_fmul: test_fmul.o fmul.o $(LIBS)
	$(LD) -o $@ $^ $(LDFLAGS)

test_finv: test_finv.o table.o finv.o fadd.o fmul.o $(LIBS)
	$(LD) -o $@ $^ $(LDFLAGS)

test_fsqrt: test_fsqrt.o table.o fsqrt.o fadd.o fmul.o $(LIBS)
	$(LD) -o $@ $^ $(LDFLAGS)
maketable_finv: maketable_finv.c def.c
	$(LD) -o $@ $^ $(LDFLAGS)
maketable_fsqrt: maketable_fsqrt.c def.c
	$(LD) -o $@ $^ $(LDFLAGS)

finv_table.dat: maketable_finv
	./maketable_finv
fsqrt_table.dat: maketable_fsqrt
	./maketable_fsqrt

table.vhd: finv_table.dat fsqrt_table.dat
	ruby table.vhd.erb

table.c: finv_table.dat fsqrt_table.dat
	ruby table.c.erb

test_finv_all: table.o test_finv_all.o finv.o $(LIBS)
	$(LD) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(EXES) $(TESTS) $(TXTS) *.o *.dat *~ work-obj93.cf \
  maketable_finv maketable_fsqrt table.c table.vhd

.PHONY: all clean $(TESTBENCH)

gen_input: ftrc.o itof.o gen_input.o def.o

work-obj93.cf: $(SOURCES)
	$(GHDLC) -i $(GHDLFLAGS) $(SOURCES)

makeanswer_fadd.o: fadd.c
makeanswer_fadd: makeanswer_fadd.o
	$(CC) $^ -o $@ $(CFLAGS) -lm
makeanswer_finv.o: finv.c
makeanswer_finv: makeanswer_finv.o table.o
	$(CC) $^ -o $@ $(CFLAGS) -lm
makeanswer_fsqrt.o: fsqrt.c
makeanswer_fsqrt: makeanswer_fsqrt.o table.o
	$(CC) $^ -o $@ $(CFLAGS) -lm
testcase.txt: maketestcase
	./maketestcase
testcase-mono.txt: maketestcase_mono
	./maketestcase_mono

test_fadd_diff: test_fadd_c test_fadd_vhdl
	diff answer.txt fadd_test/result.txt
test_fadd_c: makeanswer_fadd testcase.txt
	./makeanswer_fadd
test_fadd_vhdl: work-obj93.cf testcase.txt
	$(GHDLC) -m $(GHDLFLAGS) fadd_tb
	-mkdir fadd_test/
	-cp testcase.txt fadd_test/testcase.txt
	$(GHDLC) -r $(GHDLFLAGS) fadd_tb


test_finv_diff: test_finv_c test_finv_vhdl
	diff answer.txt finv_test/result.txt
test_finv_c: testcase-mono.txt makeanswer_finv
	./makeanswer_finv
test_finv_vhdl: work-obj93.cf testcase-mono.txt
	$(GHDLC) -m $(GHDLFLAGS) finv_tb
	-mkdir finv_test/
	-cp testcase-mono.txt finv_test/testcase.txt
	$(GHDLC) -r $(GHDLFLAGS) finv_tb --wave=finv.ghw

test_fsqrt_diff: test_fsqrt_c test_fsqrt_vhdl
	diff answer.txt fsqrt_test/result.txt
test_fsqrt_c: testcase-mono.txt makeanswer_fsqrt
	./makeanswer_fsqrt
test_fsqrt_vhdl: work-obj93.cf testcase-mono.txt
	$(GHDLC) -m $(GHDLFLAGS) fsqrt_tb
	-mkdir fsqrt_test/
	-cp testcase-mono.txt fsqrt_test/testcase.txt
	$(GHDLC) -r $(GHDLFLAGS) fsqrt_tb

$(TESTBENCH): work-obj93.cf gen_input
	$(GHDLC) -m $(GHDLFLAGS) $@
	./gen_input $@ | $(GHDLC) -r $(GHDLFLAGS) $@ $(GHDL_SIM_OPT) --wave=$@.ghw
