
ARMGNU = arm-none-linux-gnueabi

COPS = -Wall -O2 -mthumb -nostdlib -nostartfiles -ffreestanding 
AOPS = --warn --fatal-warnings


OOPS = -std-compile-opts -strip-debug
LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=thumb 


all : gxteatest.bin lxteatest.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

putget.o : putget.s
	$(ARMGNU)-as putget.s -o putget.o

guart.o : uart.c
	$(ARMGNU)-gcc $(COPS) -c uart.c -o guart.o

gembedded.o : embedded.c
	$(ARMGNU)-gcc $(COPS) -c embedded.c -o gembedded.o

gxteatest.o : xteatest.c
	$(ARMGNU)-gcc $(COPS) -c xteatest.c -o gxteatest.o

gxteatest.bin : memmap vectors.o putget.o gembedded.o gxteatest.o guart.o
	$(ARMGNU)-ld -T memmap vectors.o putget.o gembedded.o gxteatest.o guart.o -o gxteatest.elf
	$(ARMGNU)-objdump -D gxteatest.elf > gxteatest.list
	$(ARMGNU)-objcopy gxteatest.elf gxteatest.bin -O binary


embedded.bc : embedded.c
	clang $(LOPS) -c embedded.c -o embedded.bc

uart.bc : uart.c
	clang $(LOPS) -c uart.c -o uart.bc

xteatest.bc : xteatest.c
	clang $(LOPS) -c xteatest.c -o xteatest.bc

lxteatest.bin : memmap vectors.o putget.o embedded.bc xteatest.bc uart.bc
	llvm-link embedded.bc xteatest.bc uart.bc -o lxteatest.raw.bc
	opt $(OOPS) lxteatest.raw.bc -o lxteatest.bc
	llc $(LLCOPS) lxteatest.bc -o lxteatest.s
	$(ARMGNU)-as lxteatest.s -o lxteatest.o
	$(ARMGNU)-ld -T memmap vectors.o putget.o lxteatest.o -o lxteatest.elf
	$(ARMGNU)-objdump -D lxteatest.elf > lxteatest.list
	$(ARMGNU)-objcopy lxteatest.elf -O binary lxteatest.bin 



clean:
	rm -f *.bin
	rm -f *.bc
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f lxteatest.s

