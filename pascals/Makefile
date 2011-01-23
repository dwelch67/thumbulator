

ARMGNU = arm-none-linux-gnueabi

COPS = -Wall -O2 -mthumb -nostdlib -nostartfiles -ffreestanding 

AOPS = --warn --fatal-warnings


all : ptest1.bin ptest2.bin ptest3.bin ptest4.bin

ptest1.bin : vectors.s ptest1.p.s
	$(ARMGNU)-as vectors.s -o vectors.o
	$(ARMGNU)-as ptest1.p.s -o ptest1.p.o
	$(ARMGNU)-ld -T memmap vectors.o ptest1.p.o -o ptest1.elf
	$(ARMGNU)-objdump -D ptest1.elf > ptest1.list
	$(ARMGNU)-objcopy ptest1.elf ptest1.bin -O binary

ptest1.p.s : ptest1.p pthumb
	./pthumb ptest1.p

ptest1.p : pascals test1.pas
	./pascals test1.pas > ptest1.p


ptest2.bin : vectors.s ptest2.p.s
	$(ARMGNU)-as vectors.s -o vectors.o
	$(ARMGNU)-as ptest2.p.s -o ptest2.p.o
	$(ARMGNU)-ld -T memmap vectors.o ptest2.p.o -o ptest2.elf
	$(ARMGNU)-objdump -D ptest2.elf > ptest2.list
	$(ARMGNU)-objcopy ptest2.elf ptest2.bin -O binary

ptest2.p.s : ptest2.p pthumb
	./pthumb ptest2.p

ptest2.p : pascals test2.pas
	./pascals test2.pas > ptest2.p


ptest3.bin : vectors.s ptest3.p.s
	$(ARMGNU)-as vectors.s -o vectors.o
	$(ARMGNU)-as ptest3.p.s -o ptest3.p.o
	$(ARMGNU)-ld -T memmap vectors.o ptest3.p.o -o ptest3.elf
	$(ARMGNU)-objdump -D ptest3.elf > ptest3.list
	$(ARMGNU)-objcopy ptest3.elf ptest3.bin -O binary

ptest3.p.s : ptest3.p pthumb
	./pthumb ptest3.p

ptest3.p : pascals test3.pas
	./pascals test3.pas > ptest3.p




ptest4.bin : vectors.s ptest4.p.s
	$(ARMGNU)-as vectors.s -o vectors.o
	$(ARMGNU)-as ptest4.p.s -o ptest4.p.o
	$(ARMGNU)-ld -T memmap vectors.o ptest4.p.o -o ptest4.elf
	$(ARMGNU)-objdump -D ptest4.elf > ptest4.list
	$(ARMGNU)-objcopy ptest4.elf ptest4.bin -O binary

ptest4.p.s : ptest4.p pthumb
	./pthumb ptest4.p

ptest4.p : pascals test4.pas
	./pascals test4.pas > ptest4.p






pthumb : pthumb.c
	clang -O2 pthumb.c -o pthumb

pascals : pascals.pas
	gpc pascals.pas -o pascals	

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.elf
	rm -f *.list
	rm -f *.p.s
	rm -f *.p
	rm -f pascals
	rm -f pthumb






