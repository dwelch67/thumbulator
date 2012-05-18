
adc r1,r2
add r1,r2,r3
add r2,#0x69
add r2,r3,#5

adc r1 , r2
add r1 , r2 , r3
add r2 , #0x69
add r2 , r3 , #5

add r2,r11
add r11,r2
add r2,pc,#8
add r2,sp,#0x3FC

add sp , #0x10
and r1,r2
and r2,r7

asr r1,r2
asr r1,r2,#3
asr r1,r2, # 1
asr r2,r3,#32
asr r2 , r3 , #32

;b label

bic r7,r6
bkpt 123
bx r1
bx r15
cmn r7,r7
cmn r5,r2
cmp r4,#0x45
cmp r7,r3
cmp r14,r13
eor r2,r6

ldmia r2!,{r3,r1,r7}

ldr r3,[r4]
ldr r3,[r4,r5]
ldr r3,[r4,#0x10]
ldr r5,[pc]
ldr r6,[sp]
ldr r1,[pc,#0x10]
ldr r0,[sp,#8]

ldrb r5,[r6]
ldrb r5,[r7,#8]
ldrb r5,[r3, # 3]

ldrsb r1,[r2,r3]
ldrsb r7,[r6,r5]
ldrsh r3,[r4,r5]

