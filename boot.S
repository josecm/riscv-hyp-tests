
.section .boot, "ax" 
.globl _boot
_boot:
    bnez a0, .
    csrwi    sscratch, 0xf
    /* Initialize stack pointer */
    la sp, __stack_top

    /* Clear bss */ 
    la a0, __bss_start
    la a1, __bss_end
    bgeu a0, a1, 2f
1:
    sw zero, (a0)
    addi a0, a0, 4
    bltu a0, a1, 1b
2:
    call _init
    call main
    li a0, 0
    call exit
    j .

.section ".tohost","aw",@progbits
.align 6
.globl tohost
tohost: .dword 0
.align 6
.globl fromhost
fromhost: .dword 0
