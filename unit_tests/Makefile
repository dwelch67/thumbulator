
#
# Set the processor variant.
#
VARIANT=cm0

#
# Include the common make definitions.
#
include makedefs

CFLAGS+=-g
IPATH=.

LDFLAGS+=-Map ${COMPILER}/minimal.map

#
# The default rule
#
all: startup_gcc.c  axf

axf: ${COMPILER}
axf: ${COMPILER}/minimal.axf

#
# The rule to clean out all the build products.
#
clean:
	@rm -rf ${COMPILER} ${wildcard *~}
	mkdir ${COMPILER}

#
# The rule to create the target directory.
#
${COMPILER}:
	@mkdir -p ${COMPILER}

${COMPILER}/minimal.axf: ${COMPILER}/minimal.o
${COMPILER}/minimal.axf: ${COMPILER}/startup_${COMPILER}.o

#
# Source Code that allocates big chunks of RAM.
# 

# ${COMPILER}/dib.axf: ${ROOT}/driverlib/${COMPILER}/libdriver.a
${COMPILER}/minimal.axf: unittests.ld
SCATTERgcc_minimal=unittests.ld
ENTRY=ResetISR

#
# Include the automatically generated dependency files.
#
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif


# hexdump -v -e '"%08x,\n"' gcc/minimal.bin
