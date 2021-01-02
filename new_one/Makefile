
ARMGNU ?= arm-none-eabi

XCPU = -mcpu=cortex-m0

AOPS = --warn --fatal-warnings $(XCPU)
COPS = -Wall -O2 -ffreestanding $(XCPU)
LOPS = -nostdlib -nostartfiles

all : notmain.bin thumbulator

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f thumbulator

flash.o : flash.s
	$(ARMGNU)-as $(AOPS) flash.s -o flash.o

notmain.o : notmain.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c notmain.c -o notmain.o

notmain.bin :               flash.ld flash.o notmain.o
	$(ARMGNU)-ld $(LOPS) -T flash.ld flash.o notmain.o -o notmain.elf
	$(ARMGNU)-objdump -D notmain.elf > notmain.list
	$(ARMGNU)-objcopy -O binary notmain.elf notmain.bin

thumbulator: thumbulator.c
	gcc -O2 thumbulator.c -o thumbulator
