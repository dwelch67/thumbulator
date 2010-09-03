
ARMGNU = arm-none-linux-gnueabi

COPS = -Wall -O2 -mthumb -nostdlib -nostartfiles -ffreestanding 


AOPS = --warn --fatal-warnings

OOPS = -std-compile-opts -strip-debug

LOPS = -Wall -m32 -emit-llvm 

LLCOPS = -march=thumb 




all : notmain.bin lnotmain.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

putget.o : putget.s
	$(ARMGNU)-as putget.s -o putget.o

regress.o : regress.s
	$(ARMGNU)-as regress.s -o regress.o

gress.o : gress.c 
	$(ARMGNU)-gcc $(COPS) -c gress.c -o gress.o


notmain.x : notmain.c
	$(ARMGNU)-gcc $(COPS) -march=armv7m -mcpu=cortex-m3 -c notmain.c -o notmain2.o

notmain.o : notmain.c 
	$(ARMGNU)-gcc $(COPS) -c notmain.c -o notmain2.o

notmain.bin : vectors.o putget.o regress.o gress.o notmain.c memmap
	$(ARMGNU)-gcc $(COPS) vectors.o putget.o regress.o gress.o notmain.c -T memmap -o notmain.elf
	$(ARMGNU)-objdump -D notmain.elf > notmain.list
	$(ARMGNU)-objcopy notmain.elf notmain.bin -O binary

lnotmain.bin : vectors.o putget.o regress.o lnotmain.o memmap
	$(ARMGNU)-ld vectors.o putget.o regress.o lnotmain.o -T memmap -o lnotmain.elf
	$(ARMGNU)-objdump -D lnotmain.elf > lnotmain.list
	$(ARMGNU)-objcopy lnotmain.elf lnotmain.bin -O binary

lnotmain.o : notmain.c gress.c
	clang $(LOPS) -c notmain.c -o lnotmain.raw.bc
	clang $(LOPS) -c gress.c -o gress.raw.bc
	llvm-link lnotmain.raw.bc gress.raw.bc -f -o lnotmain.pre.bc
	opt $(OOPS) lnotmain.pre.bc -f -o lnotmain.bc
	llc $(LLCOPS) lnotmain.bc -o lnotmain.s
	$(ARMGNU)-as lnotmain.s -o lnotmain.o	

clean:
	rm -f *.bin
	rm -f *.bc
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f lnotmain.s

