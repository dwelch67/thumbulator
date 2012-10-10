
#ARMGNU = arm-none-linux-gnueabi
ARMGNU = arm-none-eabi

COPS = -Wall -O2 -mthumb -nostdlib -nostartfiles -ffreestanding 
AOPS = --warn --fatal-warnings


OOPS = -std-compile-opts -strip-debug
LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=thumb 


all : gteatest.bin lteatest.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

putget.o : putget.s
	$(ARMGNU)-as putget.s -o putget.o

guart.o : uart.c
	$(ARMGNU)-gcc $(COPS) -c uart.c -o guart.o

gembedded.o : embedded.c
	$(ARMGNU)-gcc $(COPS) -c embedded.c -o gembedded.o

gteatest.o : teatest.c
	$(ARMGNU)-gcc $(COPS) -c teatest.c -o gteatest.o

gteatest.bin : memmap vectors.o putget.o gembedded.o gteatest.o guart.o
	$(ARMGNU)-ld -T memmap vectors.o putget.o gembedded.o gteatest.o guart.o -o gteatest.elf
	$(ARMGNU)-objdump -D gteatest.elf > gteatest.list
	$(ARMGNU)-objcopy gteatest.elf gteatest.bin -O binary


embedded.bc : embedded.c
	clang $(LOPS) -c embedded.c -o embedded.bc

uart.bc : uart.c
	clang $(LOPS) -c uart.c -o uart.bc

teatest.bc : teatest.c
	clang $(LOPS) -c teatest.c -o teatest.bc

lteatest.bin : memmap vectors.o putget.o embedded.bc teatest.bc uart.bc
	llvm-link embedded.bc teatest.bc uart.bc -o lteatest.raw.bc
	opt $(OOPS) lteatest.raw.bc -o lteatest.bc
	llc $(LLCOPS) lteatest.bc -o lteatest.s
	$(ARMGNU)-as lteatest.s -o lteatest.o
	$(ARMGNU)-ld -T memmap vectors.o putget.o lteatest.o -o lteatest.elf
	$(ARMGNU)-objdump -D lteatest.elf > lteatest.list
	$(ARMGNU)-objcopy lteatest.elf -O binary lteatest.bin 



clean:
	rm -f *.bin
	rm -f *.bc
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f lteatest.s

