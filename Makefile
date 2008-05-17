CFLAGS := -mmcu=msp430x2234 -g -Wall
CFLAGS += -mendup-at=main
LDFLAGS :=

CC := msp430-gcc

C_FILES := main.c ir-rx.c freq.c net-rx.c opamp-1.c ir-tx.c \
	ir-tx-data.c net-tx.c adc10.c random.c motor.c virus.c \
	smbus_pec.c battery.c
H_FILES := device.h ir-rx.h freq.h net-rx.h opamp-1.h ir-bias.h \
	ir-tx.h ir-tx-data.h net-tx.h adc10.h random.h motor.h net.h \
	leds.h virus.h smbus_pec.h battery.h

include .config

ifeq ($(strip $(CONF_TX_SEQ)),y)
CFLAGS += -DCONF_TX_SEQ=1
endif

# Number of symbols we have to transmit per byte
SYM_PER_BYTE := `./.sym_per_bit.py $(NBITS)`

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
	sed -i -e "s/_SYMBOLS_PER_BYTE/$(SYM_PER_BYTE)/" freq.h

.PHONY: clean tx-test

tx-test: freq.c freq.h
	$(MAKE) -C ./tx-test

clean: 
	-rm -f main freq.{h,c}
	$(MAKE) -C ./tx-test clean

