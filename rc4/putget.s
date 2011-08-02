
;@-----------------------
;@ .cpu cortex-m3
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
.end
;@-----------------------
