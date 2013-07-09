
all : host gpicojpegtest.bin lpicojpegtest.bin

host : host.c picojpeg.c picojpeg.h jpegdata.h
	gcc -O2 host.c picojpeg.c -o host


jpegdata.h : bintoc.c
	gcc -O2 bintoc.c -o bintoc
	./bintoc


clean :
	rm -rf host
	rm -rf out.bmp
	rm -rf bintoc
	rm -rf jpegdata.h
	rm -f *.bin
	rm -f *.bc
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f lpicojpegtest.s




#ARMGNU = arm-none-linux-gnueabi
ARMGNU = arm-none-eabi

COPS = -Wall -O2 -mthumb -nostdlib -nostartfiles -ffreestanding 
AOPS = --warn --fatal-warnings


OOPS = -std-compile-opts -strip-debug -disable-simplify-libcalls
LOPS = -Wall -m32 -emit-llvm -disable-simplify-libcalls
LLCOPS = -march=thumb -disable-simplify-libcalls 


xall : gpicojpegtest.bin lpicojpegtest.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

putget.o : putget.s
	$(ARMGNU)-as putget.s -o putget.o

guart.o : uart.c
	$(ARMGNU)-gcc $(COPS) -c uart.c -o guart.o

gembedded.o : embedded.c
	$(ARMGNU)-gcc $(COPS) -c embedded.c -o gembedded.o

gpicojpegtest.o : picojpegtest.c picojpeg.h jpegdata.h
	$(ARMGNU)-gcc $(COPS) -c picojpegtest.c -o gpicojpegtest.o

gpicojpeg.o : picojpeg.c picojpeg.h
	$(ARMGNU)-gcc $(COPS) -c picojpeg.c -o gpicojpeg.o

gpicojpegtest.bin : memmap vectors.o putget.o gembedded.o gpicojpegtest.o gpicojpeg.o guart.o
	$(ARMGNU)-ld -T memmap vectors.o putget.o gembedded.o gpicojpegtest.o gpicojpeg.o guart.o -o gpicojpegtest.elf
	$(ARMGNU)-objdump -D gpicojpegtest.elf > gpicojpegtest.list
	$(ARMGNU)-objcopy gpicojpegtest.elf gpicojpegtest.bin -O binary


embedded.bc : embedded.c
	clang $(LOPS) -c embedded.c -o embedded.bc

uart.bc : uart.c
	clang $(LOPS) -c uart.c -o uart.bc

picojpegtest.bc : picojpegtest.c picojpeg.h jpegdata.h
	clang $(LOPS) -c picojpegtest.c -o picojpegtest.bc

picojpeg.bc : picojpeg.c picojpeg.h
	clang $(LOPS) -c picojpeg.c -o picojpeg.bc

lpicojpegtest.bin : memmap vectors.o putget.o embedded.bc picojpegtest.bc picojpeg.bc uart.bc
	llvm-link embedded.bc picojpegtest.bc uart.bc picojpeg.bc -o lpicojpegtest.raw.bc
	opt $(OOPS) lpicojpegtest.raw.bc -o lpicojpegtest.bc
	#llc $(LLCOPS) lpicojpegtest.bc -o lpicojpegtest.s
	#$(ARMGNU)-as lpicojpegtest.s -o lpicojpegtest.o
	llc $(LLCOPS) lpicojpegtest.raw.bc -filetype obj -o lpicojpegtest.o
	$(ARMGNU)-ld -T memmap vectors.o putget.o lpicojpegtest.o -o lpicojpegtest.elf
	$(ARMGNU)-objdump -D lpicojpegtest.elf > lpicojpegtest.list
	$(ARMGNU)-objcopy lpicojpegtest.elf -O binary lpicojpegtest.bin 


