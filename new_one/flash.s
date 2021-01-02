
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
.word systick_handler

.thumb_func
reset:
	bl notmain
    b hang

.thumb_func
hang:   
	bkpt
	/*.hword 0xBF00*/
	b .

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

.type dummy,%function
.globl dummy
dummy:
	bx lr


.thumb_func
systick_handler:
	.hword 0xBF00	
	bkpt

