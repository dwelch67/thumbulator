.word 0x40080000
.word 0x11
.word 0
.word 0

;@
;@v0 low
;@v1 low
;@i low
;@
;@delta
;@k0
;@k1
;@k2
;@k3
;@sum
;@
ldr r0,delta_add
ldmia r0!,{r1,r2,r3,r4,r5}
mov r8,r1 ;@ delta
mov r9,r2  ;@ key0
mov r10,r3 ;@ key1
mov r11,r4 ;@ key2
mov r12,r5 ;@ key3

ldr r4,data0 ;@v0
ldr r5,data1 ;@v1

mov r0,#32 ;@i
mov r1,#0 ;@sum

encrypt:
;@        sum += delta;@
    add r1,r8
;@        v0 += ((v1<<4) + k0) ^ (v1 + sum) ^ ((v1>>5) + k1);@
    lsl r2,r5,#4
    add r2,r9
    add r3,r5,r1
    eor r2,r3
    lsr r3,r5,#5
    add r3,r10
    eor r2,r3

    add r4,r4,r2
;@        v1 += ((v0<<4) + k2) ^ (v0 + sum) ^ ((v0>>5) + k3);@
    lsl r2,r4,#4
    add r2,r11
    add r3,r4,r1
    eor r2,r3
    lsr r3,r4,#5
    add r3,r12
    eor r2,r3

    add r5,r5,r2

    sub r0,#1
    bne encrypt


mov r2,#0xD0
lsl r2,r2,#24
str r4,[r2,#0x20]
str r5,[r2,#0x20]

mov r0,#32 ;@i
mov r1,r8
lsl r1,r1,#5 ;@sum = delta * 32 rounds
decrypt:
        ;@ v1 -= ((v0<<4) + k2) ^ (v0 + sum) ^ ((v0>>5) + k3);
    lsl r2,r4,#4
    add r2,r11
    add r3,r4,r1
    eor r2,r3
    lsr r3,r4,#5
    add r3,r12
    eor r2,r3

    sub r5,r5,r2

        ;@ v0 -= ((v1<<4) + k0) ^ (v1 + sum) ^ ((v1>>5) + k1);
    lsl r2,r5,#4
    add r2,r9
    add r3,r5,r1
    eor r2,r3
    lsr r3,r5,#5
    add r3,r10
    eor r2,r3

    sub r4,r4,r2
        ;@ sum -= delta;

    mov r2,r8
    sub r1,r1,r2
    ;@v[0]=v0; v[1]=v1;

    sub r0,#1
    bne decrypt

mov r2,#0xD0
lsl r2,r2,#24
str r4,[r2,#0x20]
str r5,[r2,#0x20]


mov r2,#0xF0
lsl r2,r2,#24
str r2,[r2] ;@ write to 0xF0000000 to halt thumbulator

hang: b hang

.align
delta_add: .word =delta
delta: .word 0x9E3779B9
key0: .word 0x0000091A
key1: .word 0x0000048D
key2: .word 0xBF9EC2DF
key3: .word 0xE051A1F6

data0: .word 0x7028D0FB
data1: .word 0x878AA8E4
