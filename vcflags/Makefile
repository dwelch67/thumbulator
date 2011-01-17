

ARMGNU = arm-none-linux-gnueabi

COPS = -Wall -O2 -mthumb -nostdlib -nostartfiles -ffreestanding 

AOPS = --warn --fatal-warnings

all : vcflags tvcflags.bin

vcflags : vcflags.c someasm.s
	arm-none-linux-gnueabi-gcc vcflags.c someasm.s -o vcflags


tvcflags.bin : vectors.s putget.s tvcflags.c 
	$(ARMGNU)-as vectors.s -o vectors.o
	$(ARMGNU)-as putget.s -o putget.o
	$(ARMGNU)-gcc $(COPS) -c tvcflags.c -o tvcflags.o
	$(ARMGNU)-ld -T memmap vectors.o putget.o tvcflags.o -o tvcflags.elf
	$(ARMGNU)-objdump -D tvcflags.elf > tvcflags.list
	$(ARMGNU)-objcopy tvcflags.elf tvcflags.bin -O binary

clean :
	rm -f vcflags
	rm -f *.o
	rm -f *.bin
	rm -f *.elf
	rm -f *.list






