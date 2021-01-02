

.cpu cortex-m0
.thumb


.thumb_func
.global _start
_start:
stacktop: .word 0x20001000
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
    bl notmain
    b hang
.thumb_func
hang:   
	bkpt
	b .

.align

.thumb_func
.globl PUT16
PUT16:
    strh r1,[r0]
    bx lr

.thumb_func
.globl PUT32
PUT32:
    str r1,[r0]
    bx lr

.thumb_func
.globl GET32
GET32:
    ldr r0,[r0]
    bx lr

.thumb_func
.globl GET16
GET16:
    ldrh r0,[r0]
    bx lr

.thumb_func
.globl dummy
dummy:
    bx lr

.balign 0x100
.thumb_func
.globl TEST
TEST:
	bx pc
	add r0,#1
	add r0,#1
	add r0,#1
	add r0,#1
	add r0,#1
	add r0,#1
	add r0,#1
	add r0,#1
	add r0,#1
	bx lr









