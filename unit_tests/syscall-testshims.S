@ Rudimentary functions for testing the 
@ system call interface.
@ Pair these up with C language .h files
@ for testing.

.text
.syntax unified

.thumb_func     @  Make sure the linker puts it in the table.
.global PutChar
PutChar:
	svc #2
	bx lr

.thumb_func     @  Make sure the linker puts it in the table.
.global GetChar
GetChar:
	svc #3
	bx lr

.thumb_func     
.global GetCharAvail
GetCharAvail:
	svc #4
	bx lr

