CFLAGS := -mmcu=msp430x2001 -g -Wall
LDFLAGS :=

CC := msp430-gcc

C_FILES := main.c timer-a.c net.c ../freq.c
H_FILES := device.h timer-a.h net.h ../freq.h

main: $(C_FILES) $(H_FILES)
	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) $(C_FILES)


.PHONY: clean

clean: 
	-rm -f main
