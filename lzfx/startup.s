.set    noreorder


.globl _start
_start:
    li $sp,0x10000
    jal notmain
    li $18,3

    /* halt the sim */
    .word 0x000000FF

hang:
    j hang
    nop

.globl PUT32
PUT32:
    jr $ra
    sw $a1,0($a0)

.globl GET32
GET32:
    jr $ra
    lw $v0,0($a0)

.globl dummy
dummy:
    jr $ra
    nop

.set    reorder
