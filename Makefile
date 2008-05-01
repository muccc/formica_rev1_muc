CFLAGS := -mmcu=msp430x2234 -g -Wall -Os
CFLAGS += -mendup-at=main
LDFLAGS :=

CC := msp430-gcc

C_FILES := main.c ir-receive.c freq.c net-rx.c opamp-1.c
H_FILES := device.h ir-receive.h freq.h net-rx.h opamp-1.h

include .config

main: $(C_FILES) $(H_FILES)
	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) $(C_FILES)

freq.c: freq.py .config
	./freq.py $(MIN_PERIOD) $(MAX_PERIOD) $(NBITS) > freq.c

freq.h: .freq.h .config
	cp .freq.h freq.h
	sed -i -e "s/_NFREQ/$(NFREQ)/" freq.h
	sed -i -e "s/_NBITS/$(NBITS)/" freq.h
	sed -i -e "s/_MIN_PERIOD/$(MIN_PERIOD)/" freq.h
	sed -i -e "s/_MAX_PERIOD/$(MAX_PERIOD)/" freq.h

.PHONY: clean tx-test

tx-test: freq.c freq.h
	$(MAKE) -C ./tx-test

clean: 
	-rm -f main freq.{h,c}
	$(MAKE) -C ./tx-test clean

