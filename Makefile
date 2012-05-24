
thumbulator : thumbulator.c
	gcc -o thumbulator -O2 thumbulator.c

clean :
	rm -f thumbulator
	rm -f output.vcd




