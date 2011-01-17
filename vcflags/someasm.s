
.globl testfun1
testfun1:
    ;@ clear carry flag
    mov r2,#0
    adds r2,r2
    adcs r2,r0,r1
    mrs r0,cpsr
    bx lr

.globl testfun2
testfun2:
    ;@ set carry flag
    mvn r2,#0
    adds r2,r2
    adcs r2,r0,r1
    mrs r0,cpsr
    bx lr

.globl testfun3
testfun3:
    ;@ clear carry flag
    mov r2,#0
    adds r2,r2
    sbcs r2,r0,r1
    mrs r0,cpsr
    bx lr

.globl testfun4
testfun4:
    ;@ set carry flag
    mvn r2,#0
    adds r2,r2
    sbcs r2,r0,r1
    mrs r0,cpsr
    bx lr
