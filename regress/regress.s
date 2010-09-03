
;@-----------------------
.thumb
;@-----------------------
.thumb_func
.globl ROR
ROR:
    ror r0,r1
    bx lr

.thumb_func
.globl ROR_FLAG
ROR_FLAG:
    ror r1,r2
    mov r0,#0
    mov r1,#1
    bcs 1f
    bcc 2f
1:  orr r0,r1
    b 3f
2:  orr r0,r1
3:
    mov r1,#2
    beq 1f
    bne 2f
1:  orr r0,r1
    b 3f
2:  orr r0,r1
3:
    mov r1,#4
    bmi 1f
    bpl 2f
1:  orr r0,r1
    b 3f
2:  orr r0,r1
3:
    bx lr


.thumb_func
.globl LSR
LSR:
    lsr r0,r1
    bx lr


.thumb_func
.globl LSR_FLAG
LSR_FLAG:
    lsr r1,r2
    mov r0,#0
    mov r1,#1
    bcs 1f
    bcc 2f
1:  orr r0,r1
    b 3f
2:  orr r0,r1
3:
    mov r1,#2
    beq 1f
    bne 2f
1:  orr r0,r1
    b 3f
2:  orr r0,r1
3:
    mov r1,#4
    bmi 1f
    bpl 2f
1:  orr r0,r1
    b 3f
2:  orr r0,r1
3:
    bx lr

.thumb_func
.globl LSL
LSL:
    lsl r0,r1
    bx lr


.thumb_func
.globl LSL_FLAG
LSL_FLAG:
    lsl r1,r2
    mov r0,#0
    mov r1,#1
    bcs 1f
    bcc 2f
1:  orr r0,r1
    b 3f
2:  orr r0,r1
3:
    mov r1,#2
    beq 1f
    bne 2f
1:  orr r0,r1
    b 3f
2:  orr r0,r1
3:
    mov r1,#4
    bmi 1f
    bpl 2f
1:  orr r0,r1
    b 3f
2:  orr r0,r1
3:
    bx lr



;@-----------------------
.end
;@-----------------------
