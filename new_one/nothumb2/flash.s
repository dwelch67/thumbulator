
.cpu cortex-m0
.thumb

.thumb_func
.global _start
_start:
	stacktop: .word 0x20001000
	.word reset

.thumb_func
reset:
	nop
	nop
	dmb
	bkpt
	bkpt
	bkpt
