
thumbulator : thumbulator.c svc_stdio.c
	gcc -o thumbulator -O2 -DSRAMBASE=0x20000000 thumbulator.c svc_stdio.c

clean :
	rm -f thumbulator
	rm -f output.vcd




