
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings
COPS = -mthumb -mcpu=arm7tdmi -march=armv4t -Wall -Werror -Os -nostdlib -nostartfiles -nodefaultlibs -nostdlib -ffreestanding

LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=thumb
OOPS = -std-compile-opts


all : adventure.gcc.bin adventure.clang.norm.bin

gcc : adventure.gcc.bin

llvm : adventure.clang.norm.bin adventure.clang.opt.bin


vectors.o : vectors.s
	$(ARMGNU)-as $(AOPS) vectors.s -o vectors.o

clean :
	rm -f *.o
	rm -f *.elf
	rm -f *.bin
	rm -f *.list
	rm -f *.norm.s
	rm -f *.opt.s
	rm -f *.bc



adventure.gcc.bin : adventure.gcc.elf
	$(ARMGNU)-objcopy adventure.gcc.elf -O binary adventure.gcc.bin

adventure.gcc.elf : vectors.o uart.o crc.o data.o done.o init.o io.o main.o show.o subr.o vocab.o wizard.o
	$(ARMGNU)-gcc $(COPS) -Xlinker memmap -o adventure.gcc.elf vectors.o uart.o crc.o data.o done.o init.o io.o main.o show.o subr.o vocab.o wizard.o -lgcc
	#$(ARMGNU)-ld -T memmap -o adventure.gcc.elf vectors.o uart.o crc.o data.o done.o init.o io.o main.o show.o subr.o vocab.o wizard.o
	$(ARMGNU)-objdump -D adventure.gcc.elf > adventure.gcc.list


uart.o : uart.c
	$(ARMGNU)-gcc -c $(COPS) uart.c -o uart.o

crc.o : crc.c
	$(ARMGNU)-gcc -c $(COPS) crc.c -o crc.o

data.o : data.c
	$(ARMGNU)-gcc -c $(COPS) data.c -o data.o

done.o : done.c
	$(ARMGNU)-gcc -c $(COPS) done.c -o done.o

init.o : init.c
	$(ARMGNU)-gcc -c $(COPS) init.c -o init.o

io.o : io.c
	$(ARMGNU)-gcc -c $(COPS) io.c -o io.o

main.o : main.c
	$(ARMGNU)-gcc -c $(COPS) main.c -o main.o

show.o : show.c
	$(ARMGNU)-gcc -c $(COPS) show.c -o show.o

subr.o : subr.c
	$(ARMGNU)-gcc -c $(COPS) subr.c -o subr.o

vocab.o : vocab.c
	$(ARMGNU)-gcc -c $(COPS) vocab.c -o vocab.o

wizard.o : wizard.c
	$(ARMGNU)-gcc -c $(COPS) wizard.c -o wizard.o




uart.clang.bc : uart.c  
	clang $(LOPS) -c uart.c -o uart.clang.bc

crc.clang.bc : crc.c  
	clang $(LOPS) -c crc.c -o crc.clang.bc

data.clang.bc : data.c  
	clang $(LOPS) -c data.c -o data.clang.bc

done.clang.bc : done.c  
	clang $(LOPS) -c done.c -o done.clang.bc

init.clang.bc : init.c  
	clang $(LOPS) -c init.c -o init.clang.bc

io.clang.bc : io.c  
	clang $(LOPS) -c io.c -o io.clang.bc

main.clang.bc : main.c  
	clang $(LOPS) -c main.c -o main.clang.bc

show.clang.bc : show.c  
	clang $(LOPS) -c show.c -o show.clang.bc

subr.clang.bc : subr.c  
	clang $(LOPS) -c subr.c -o subr.clang.bc

vocab.clang.bc : vocab.c  
	clang $(LOPS) -c vocab.c -o vocab.clang.bc

wizard.clang.bc : wizard.c  
	clang $(LOPS) -c wizard.c -o wizard.clang.bc

adventure.clang.bc : crc.clang.bc data.clang.bc done.clang.bc init.clang.bc io.clang.bc main.clang.bc show.clang.bc subr.clang.bc vocab.clang.bc wizard.clang.bc uart.clang.bc
	llvm-link crc.clang.bc data.clang.bc done.clang.bc init.clang.bc io.clang.bc main.clang.bc show.clang.bc subr.clang.bc vocab.clang.bc wizard.clang.bc uart.clang.bc -o adventure.clang.bc


adventure.clang.norm.bin : memmap vectors.o adventure.clang.bc
	llc $(LLCOPS) adventure.clang.bc -o adventure.clang.norm.s
	$(ARMGNU)-as adventure.clang.norm.s -o adventure.clang.norm.o
	$(ARMGNU)-ld -o adventure.clang.norm.elf -T memmap vectors.o adventure.clang.norm.o
	#$(ARMGNU)-gcc -o adventure.clang.norm.elf -Xlinker memmap vectors.o adventure.clang.norm.o -lgcc
	$(ARMGNU)-objdump -D adventure.clang.norm.elf > adventure.clang.norm.list
	$(ARMGNU)-objcopy adventure.clang.norm.elf adventure.clang.norm.bin -O binary

adventure.clang.opt.bin : memmap vectors.o adventure.clang.bc
	opt $(OOPS) adventure.clang.bc -o adventure.clang.opt.bc
	llc $(LLCOPS) adventure.clang.opt.bc -o adventure.clang.opt.s
	$(ARMGNU)-as adventure.clang.opt.s -o adventure.clang.opt.o
	$(ARMGNU)-ld -o adventure.clang.opt.elf -T memmap vectors.o adventure.clang.opt.o
	#$(ARMGNU)-gcc -o adventure.clang.opt.elf -Xlinker memmap vectors.o adventure.clang.opt.o -lgcc
	$(ARMGNU)-objdump -D adventure.clang.opt.elf > adventure.clang.opt.list
	$(ARMGNU)-objcopy adventure.clang.opt.elf adventure.clang.opt.bin -O binary







