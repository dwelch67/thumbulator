# This is just a way for 'Make Clean' to work
# As expected (by muscle memory)
#
# Now: A real makefile..   Who knew?

FSRC=main.fth

all: TESTBED.img 
	
TESTBED.img : $(FSRC)
	./build.sh TESTBED.img $<

clean:
	rm -f *.hex *.bin *.img *.XRF *.LOG *.astris *.img-i2c

