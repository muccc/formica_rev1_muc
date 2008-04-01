CFLAGS := -mmcu=msp430x2001 -g -Wall
LDFLAGS :=

CC := msp430-gcc

C_FILES := main.c timer-a.c
H_FILES := device.h timer-a.h

main: $(C_FILES) $(H_FILES)
	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) $(C_FILES)


.PHONY: clean

clean: 
	-rm -f main
