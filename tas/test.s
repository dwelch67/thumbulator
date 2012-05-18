
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

b 0x30
b 0x30
beq 0x30
beq 0x32
bl 0x30


b label
bl label
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

label:

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

ldrh r5,[r6]
ldrh r5,[r7,#8]
ldrh r5,[r3, # 4]

ldrsb r1,[r2,r3]
ldrsb r7,[r6,r5]
ldrsh r3,[r4,r5]

beq label
bne label
bal label

bne 0x10
beq 0x10
bal 0x10

bl label
bl 0x10

b 0x30
b 0x30
beq 0x30
beq 0x32
bl 0x30
mov r3,#0xFF
mov r2,#0x69
mov r1,r2
mov r1,r11
mov r12,r2
mul r1,r2
mvn r3,r5
neg r4,r7
orr r2,r4
pop {r1,r5,r3,r4,pc}
push {r1,r2,r4,lr}

ror r1,r2
sbc r3,r4
stmia r6!,{r4,r5,r6,r1}


str r5,[r1,#0x30]
str r1,[r2,r3]
str r4,[r6]
str r3,[sp]
str r7,[sp,#0x20]

strb r1,[r2]
strb r3,[r0,#0x10]
strb r5,[r4,r6]
strh r1,[r2]
strh r3,[r0,#0x10]
strh r5,[r4,r6]

sub r1,r2,#3
sub r3,#0xFF
sub r5,r6,r7
sub sp,#0x10
swi 0x12

tst r7,r3


