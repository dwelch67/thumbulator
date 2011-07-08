
/* vectors.s */
.thumb

.word   0x40010000  /* 0x00 stack top address */
.word   _start      /* 0x04 1 Reset */
.word   hang        /* 0x08 2 NMI */
.word   hang        /* 0x0C 3 HardFault */
.word   hang        /* 0x10 4 MemManage */
.word   hang        /* 0x14 5 BusFault */
.word   hang        /* 0x18 6 UsageFault */
.word   hang        /* 0x1C 7 RESERVED */
.word   hang        /* 0x20 8 RESERVED */
.word   hang        /* 0x24 9 RESERVED*/
.word   hang        /* 0x28 10 RESERVED */
.word   hang        /* 0x2C 11 SVCall */
.word   hang        /* 0x30 12 Debug Monitor */
.word   hang        /* 0x34 13 RESERVED */
.word   hang        /* 0x38 14 PendSV */
.word   hang        /* 0x3C 15 SysTick */
.word   hang        /* 0x40 16 External Interrupt(0) */
.word   hang        /* 0x44 17 External Interrupt(1) */
.word   hang        /* 0x48 18 External Interrupt(2) */
.word   hang        /* 0x4C 19 ...   */

.thumb_func
hang:   b .

.thumb_func
.global _start
_start:
    ldr r0,=notmain
    mov lr,pc
    bx r0
    swi 1
    @ldr r0,=0xF0000000 ;@ halt
    @str r0,[r0]
    b .


.end
