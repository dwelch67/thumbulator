
ARMGNU = arm-none-linux-gnueabi

COPS = -Wall -O2 -mthumb -nostdlib -nostartfiles -ffreestanding 


AOPS = --warn --fatal-warnings

OOPS = -std-compile-opts -strip-debug

LOPS = -Wall -m32 -emit-llvm

LLCOPS = -march=thumb 



all : notmain.bin zmain

zmain : abcmath.c main.c
	gcc -o main main.c abcmath.c
	./main

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

putget.o : putget.s
	$(ARMGNU)-as putget.s -o putget.o

randmath : randmath.c
	gcc -o randmath randmath.c

abcmath.c : randmath
	./randmath

notmain.bin : vectors.o putget.o notmain.o memmap
	$(ARMGNU)-ld -T memmap vectors.o putget.o notmain.o -o notmain.elf
	$(ARMGNU)-objdump -D notmain.elf > notmain.list
	$(ARMGNU)-objcopy notmain.elf notmain.bin -O binary


abcmath.bc : abcmath.c
	clang $(LOPS) -c abcmath.c -o abcmath.bc

notmain.bc : notmain.c
	clang $(LOPS) -c notmain.c -o notmain.bc

notmain.o : abcmath.bc notmain.bc
	llvm-link abcmath.bc notmain.bc -f -o notmain.pre.bc
	opt $(OOPS) notmain.pre.bc -f -o notmain.opt.bc
	llc $(LLCOPS) notmain.opt.bc  -o notmain.s
	$(ARMGNU)-as $(AOPS) notmain.s -o notmain.o


clean:
	rm -f *.bin
	rm -f *.bc
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f notmain.s
	rm -f randmath
	rm -f main
	rm -f abcmath.c




