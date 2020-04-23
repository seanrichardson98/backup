YCC=./yas
YIS=./yis

BEXEC=bsort
BSRC=bsort.ys
BOBJ=bsort.yo

CMOVEXEC=cmovsort
CMOVOBJ=cmov.yo
CMOVSRC=cmov.ys

CHEXEC=check
CHSRC=check.ys
CHOBJ=check.yo

CHOEXEC=checkopt
CHOSRC=checkopt.ys
CHOOBJ=checkopt.yo

CC=cc
CFLAGS=-save-temps -Og -std=c99 -Wall

CEXEC=csort
COBJS=sort.c driver.c check.c

all: $(CEXEC) cleantemps $(BEXEC) $(CMOVEXEC) $(CHEXEC) $(CHOEXEC)

$(BEXEC): $(BOBJ)
	@echo $(YIS) $< > $@
	@chmod 755 $@

$(CMOVEXEC): $(CMOVOBJ)
	@echo $(YIS) $< > $@
	@chmod 755 $@

$(CHEXEC): $(CHOBJ)
	@echo $(YIS) $< > $@
	@chmod 755 $@

$(CHOEXEC): $(CHOOBJ)
	@echo $(YIS) $< > $@
	@chmod 755 $@

$(CEXEC): $(COBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(BOBJ): $(BSRC)
	$(YCC) $<

$(CMOVOBJ): $(CMOVSRC)
	$(YCC) $<

$(CHOBJ): $(CHSRC)
	$(YCC) $<

$(CHOOBJ): $(CHOSRC)
	$(YCC) $<

clean:
	rm -f $(CMOVEXEC) $(BEXEC) $(CHEXEC) $(CHOEXEC) $(CEXEC) *.s *.o *.yo *.bc *.i

cleantemps:
	@rm -f *.i *.o
