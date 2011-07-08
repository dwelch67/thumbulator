
#ARMGNU ?= arm-none-linux-gnueabi
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -mthumb -nostdlib -nostartfiles -ffreestanding 


AOPS = --warn --fatal-warnings

OOPS = -std-compile-opts -strip-debug

LOPS = -Wall -m32 -emit-llvm 

LLCOPS = -march=thumb 


all : limit_test.bin 

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

putget.o : putget.s
	$(ARMGNU)-as putget.s -o putget.o

limit_test.x : limit_test.c
	$(ARMGNU)-gcc $(COPS) -march=armv7m -mcpu=cortex-m3 -c limit_test.c -o limit_test2.o

limit_test.bin : vectors.o putget.o limit_test.c limit_test.stuff.c memmap
	$(ARMGNU)-gcc $(COPS) vectors.o putget.o limit_test.c limit_test.stuff.c -T memmap -o limit_test.elf
	$(ARMGNU)-objdump -D limit_test.elf > limit_test.list
	$(ARMGNU)-objcopy limit_test.elf limit_test.bin -O binary

llimit_test.bin : vectors.o putget.o llimit_test.o memmap
	$(ARMGNU)-ld vectors.o putget.o llimit_test.o -T memmap -o llimit_test.elf
	$(ARMGNU)-objdump -D llimit_test.elf > llimit_test.list
	$(ARMGNU)-objcopy llimit_test.elf llimit_test.bin -O binary

llimit_test.o : limit_test.c
	clang $(LOPS) -c limit_test.c -o llimit_test.raw.bc
	opt $(OOPS) llimit_test.raw.bc -f -o llimit_test.bc
	llc $(LLCOPS) llimit_test.bc -o llimit_test.s
	$(ARMGNU)-as llimit_test.s -o llimit_test.o	

clean:
	rm -f *.bin
	rm -f *.bc
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f llimit_test.s

