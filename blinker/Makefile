
#ARMGNU = arm-none-linux-gnueabi
ARMGNU = arm-none-eabi

COPS = -Wall -O2 -mthumb -nostdlib -nostartfiles -ffreestanding 


AOPS = --warn --fatal-warnings

OOPS = -std-compile-opts -strip-debug

LOPS = -Wall -m32 

LLCOPS = -march=thumb 




all : gnotmain.bin lnotmain.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

putget.o : putget.s
	$(ARMGNU)-as putget.s -o putget.o

gnotmain.o : notmain.c 
	$(ARMGNU)-gcc $(COPS) -c notmain.c -o gnotmain.o

gnotmain.bin : vectors.o putget.o gnotmain.o memmap
	$(ARMGNU)-ld -T memmap vectors.o putget.o gnotmain.o -o gnotmain.elf
	$(ARMGNU)-objdump -D gnotmain.elf > gnotmain.list
	$(ARMGNU)-objcopy gnotmain.elf gnotmain.bin -O binary

lnotmain.bin : vectors.o putget.o lnotmain.o memmap
	$(ARMGNU)-ld vectors.o putget.o lnotmain.o -T memmap -o lnotmain.elf
	$(ARMGNU)-objdump -D lnotmain.elf > lnotmain.list
	$(ARMGNU)-objcopy lnotmain.elf lnotmain.bin -O binary

lnotmain.o : notmain.c 
	clang $(LOPS) -emit-llvm -c notmain.c -o lnotmain.raw.bc
	opt $(OOPS) lnotmain.raw.bc -f -o lnotmain.bc
	llc $(LLCOPS) lnotmain.bc  -o lnotmain.s
	$(ARMGNU)-as lnotmain.s -o lnotmain.o	

clean:
	rm -f *.bin
	rm -f *.bc
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f lnotmain.s

