
#ARMGNU ?= arm-none-eabi
ARMGNU ?= arm-linux-gnueabi

XCPU = -mcpu=cortex-m0

AOPS = --warn --fatal-warnings $(XCPU)
LOPS = -nostdlib -nostartfiles

all : notmain.bin

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

flash.o : flash.s
	$(ARMGNU)-as $(AOPS) flash.s -o flash.o

notmain.bin :               flash.ld flash.o
	$(ARMGNU)-ld $(LOPS) -T flash.ld flash.o -o notmain.elf
	$(ARMGNU)-objdump -D notmain.elf > notmain.list
	$(ARMGNU)-objcopy -O binary notmain.elf notmain.bin
