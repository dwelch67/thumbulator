
This is the classic game of adventure.  Yeah, really, on a
microcontroller. (using the 4MHz oscillator).

It needs some debugging, perhaps compiler, perhaps code, who knows.

The gcc.thumb2. version likes to work sometimes.  The others do not.

It is one of those things where you change a line of code and things
break, change another and things work.

The cpu clock has not been boosted in this version, running at the
power on

It just barely fits more than 0x3000 bytes of .bss, allocated 0x4000
synthesized mallocs, over 0x3000 bytes of mallocs, have only 0x8000
bytes of ram in a single chunk for all of that data.  There are two
other 0x4000 byte chunks.  Would like to use one for the stack but
need to read more about those other memories.  Perhaps they have to
be enabled.
