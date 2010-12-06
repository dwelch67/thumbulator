
;@-----------------------
.cpu cortex-m3
.thumb
;@-----------------------
.thumb_func
.globl PUT32
PUT32:
    str r1,[r0]
    bx lr
;@-----------------------
.thumb_func
.globl GET32
GET32:
    ldr r0,[r0]
    bx lr
;@-----------------------
.thumb_func
.globl PUTGETCLR
PUTGETCLR:
    ldr r2,[r0]
    bic r2,r1
    str r2,[r0]
    bx lr
;@-----------------------
.thumb_func
.globl PUTGETSET
PUTGETSET:
    ldr r2,[r0]
    orr r2,r1
    str r2,[r0]
    bx lr
;@-----------------------
.thumb_func
.globl atest
atest:
    add r2,r0,r1
    mov r0,#0
    bvc atest1
    mov r3,#1
    orr r0,r3
atest1:
    bcc atest2
    mov r3,#2
    orr r0,r3
atest2:
    bx lr
;@-----------------------
.thumb_func
.globl stest
stest:
    sub r2,r0,r1
    mov r0,#0
    bvc stest1
    mov r3,#1
    orr r0,r3
stest1:
    bcc stest2
    mov r3,#2
    orr r0,r3
stest2:
    bx lr
;@-----------------------
;@-----------------------




.end
;@-----------------------
