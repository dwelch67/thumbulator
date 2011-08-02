
ARMGNU = arm-none-linux-gnueabi

COPS = -Wall -O2 -mthumb -nostdlib -nostartfiles -ffreestanding 
AOPS = --warn --fatal-warnings


OOPS = -std-compile-opts -strip-debug
LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=thumb 


all : grc4test.bin lrc4test.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

putget.o : putget.s
	$(ARMGNU)-as putget.s -o putget.o

guart.o : uart.c
	$(ARMGNU)-gcc $(COPS) -c uart.c -o guart.o

gembedded.o : embedded.c
	$(ARMGNU)-gcc $(COPS) -c embedded.c -o gembedded.o

grc4test.o : rc4test.c
	$(ARMGNU)-gcc $(COPS) -c rc4test.c -o grc4test.o

grc4test.bin : memmap vectors.o putget.o gembedded.o grc4test.o guart.o
	$(ARMGNU)-ld -T memmap vectors.o putget.o gembedded.o grc4test.o guart.o -o grc4test.elf
	$(ARMGNU)-objdump -D grc4test.elf > grc4test.list
	$(ARMGNU)-objcopy grc4test.elf grc4test.bin -O binary


embedded.bc : embedded.c
	clang $(LOPS) -c embedded.c -o embedded.bc

uart.bc : uart.c
	clang $(LOPS) -c uart.c -o uart.bc

rc4test.bc : rc4test.c
	clang $(LOPS) -c rc4test.c -o rc4test.bc

lrc4test.bin : memmap vectors.o putget.o embedded.bc rc4test.bc uart.bc
	llvm-link embedded.bc rc4test.bc uart.bc -o lrc4test.raw.bc
	opt $(OOPS) lrc4test.raw.bc -o lrc4test.bc
	llc $(LLCOPS) lrc4test.bc -o lrc4test.s
	$(ARMGNU)-as lrc4test.s -o lrc4test.o
	$(ARMGNU)-ld -T memmap vectors.o putget.o lrc4test.o -o lrc4test.elf
	$(ARMGNU)-objdump -D lrc4test.elf > lrc4test.list
	$(ARMGNU)-objcopy lrc4test.elf -O binary lrc4test.bin 



clean:
	rm -f *.bin
	rm -f *.bc
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f lrc4test.s

