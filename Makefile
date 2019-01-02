
thumbulator : thumbulator.c
	gcc -o thumbulator -O2 -DSRAMBASE=0x40000000 thumbulator.c

clean :
	rm -f thumbulator
	rm -f output.vcd




