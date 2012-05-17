
ARMGNU ?= arm-none-eabi

COPS = -Wall -mthumb -nostdlib -nostartfiles -ffreestanding 
AOPS = --warn --fatal-warnings

all : lzfx_decompO0.bin lzfx_decompO1.bin lzfx_decompO2.bin lzfx_decompO3.bin

clean :
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.norm.s
	rm -f *.opt.s

vectors.o : vectors.s
	$(ARMGNU)-as $(AOPS) vectors.s -o vectors.o

putget.o : putget.s
	$(ARMGNU)-as $(AOPS) putget.s -o putget.o

lzfx_decompO0.bin : memmap vectors.o putget.o uart.c lzfx_decomp.c lzfx_decomp_test.c
	$(ARMGNU)-gcc $(COPS) -O0 -c uart.c -o uart.o
	$(ARMGNU)-gcc $(COPS) -O0 -c lzfx_decomp.c -o lzfx_decomp.o
	$(ARMGNU)-gcc $(COPS) -O0 -c lzfx_decomp_test.c -o lzfx_decomp_test.o
	$(ARMGNU)-ld $(LDOPS) -T memmap vectors.o putget.o uart.o lzfx_decomp_test.o lzfx_decomp.o -o lzfx_decompO0.elf
	$(ARMGNU)-objdump -D lzfx_decompO0.elf > lzfx_decompO0.list
	$(ARMGNU)-objcopy lzfx_decompO0.elf -O binary lzfx_decompO0.bin

lzfx_decompO1.bin : memmap vectors.o putget.o uart.c lzfx_decomp.c lzfx_decomp_test.c
	$(ARMGNU)-gcc $(COPS) -O1 -c uart.c -o uart.o
	$(ARMGNU)-gcc $(COPS) -O1 -c lzfx_decomp.c -o lzfx_decomp.o
	$(ARMGNU)-gcc $(COPS) -O1 -c lzfx_decomp_test.c -o lzfx_decomp_test.o
	$(ARMGNU)-ld $(LDOPS) -T memmap vectors.o putget.o uart.o lzfx_decomp_test.o lzfx_decomp.o -o lzfx_decompO1.elf
	$(ARMGNU)-objdump -D lzfx_decompO1.elf > lzfx_decompO1.list
	$(ARMGNU)-objcopy lzfx_decompO1.elf -O binary lzfx_decompO1.bin


lzfx_decompO2.bin : memmap vectors.o putget.o uart.c lzfx_decomp.c lzfx_decomp_test.c
	$(ARMGNU)-gcc $(COPS) -O2 -c uart.c -o uart.o
	$(ARMGNU)-gcc $(COPS) -O2 -c lzfx_decomp.c -o lzfx_decomp.o
	$(ARMGNU)-gcc $(COPS) -O2 -c lzfx_decomp_test.c -o lzfx_decomp_test.o
	$(ARMGNU)-ld $(LDOPS) -T memmap vectors.o putget.o uart.o lzfx_decomp_test.o lzfx_decomp.o -o lzfx_decompO2.elf
	$(ARMGNU)-objdump -D lzfx_decompO2.elf > lzfx_decompO2.list
	$(ARMGNU)-objcopy lzfx_decompO2.elf -O binary lzfx_decompO2.bin

lzfx_decompO3.bin : memmap vectors.o putget.o uart.c lzfx_decomp.c lzfx_decomp_test.c
	$(ARMGNU)-gcc $(COPS) -O3 -c uart.c -o uart.o
	$(ARMGNU)-gcc $(COPS) -O3 -c lzfx_decomp.c -o lzfx_decomp.o
	$(ARMGNU)-gcc $(COPS) -O3 -c lzfx_decomp_test.c -o lzfx_decomp_test.o
	$(ARMGNU)-ld $(LDOPS) -T memmap vectors.o putget.o uart.o lzfx_decomp_test.o lzfx_decomp.o -o lzfx_decompO3.elf
	$(ARMGNU)-objdump -D lzfx_decompO3.elf > lzfx_decompO3.list
	$(ARMGNU)-objcopy lzfx_decompO3.elf -O binary lzfx_decompO3.bin

