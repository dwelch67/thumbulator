
.cpu cortex-m0
.thumb

.word 0x20001000
.word reset
.word hang
.word hang

.word hang
.word hang
.word hang
.word hang

.word hang
.word hang
.word hang
.word hang

.word hang
.word hang
.word hang
.word hang

.thumb_func
reset:
	ldr r0,=0x12345678
	ldr r1,=0xD0000020
	str r0,[r1]
    b hang

.thumb_func
hang:   
	bkpt
	/*.hword 0xBF00*/
	b .
