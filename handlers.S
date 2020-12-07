#define LOAD ld
#define STORE sd
#define REGLEN (8)

.macro SAVE_CONTEXT

    add sp, sp, -(REGLEN*31)

    STORE   x1, 0*REGLEN(sp)
    STORE   x3, 2*REGLEN(sp)
    STORE   x4, 3*REGLEN(sp)
    STORE   x5, 4*REGLEN(sp)
    STORE   x6, 5*REGLEN(sp)
    STORE   x7, 6*REGLEN(sp)
    STORE   x8, 7*REGLEN(sp)
    STORE   x9, 8*REGLEN(sp)
    STORE   x10, 9*REGLEN(sp)
    STORE   x11, 10*REGLEN(sp)
    STORE   x12, 11*REGLEN(sp)
    STORE   x13, 12*REGLEN(sp)
    STORE   x14, 13*REGLEN(sp)
    STORE   x15, 14*REGLEN(sp)
    STORE   x16, 15*REGLEN(sp)
    STORE   x17, 16*REGLEN(sp)
    STORE   x18, 17*REGLEN(sp)
    STORE   x19, 18*REGLEN(sp)
    STORE   x20, 19*REGLEN(sp)
    STORE   x21, 20*REGLEN(sp)
    STORE   x22, 21*REGLEN(sp)
    STORE   x23, 22*REGLEN(sp)
    STORE   x24, 23*REGLEN(sp)
    STORE   x25, 24*REGLEN(sp)
    STORE   x26, 25*REGLEN(sp)
    STORE   x27, 26*REGLEN(sp)
    STORE   x28, 27*REGLEN(sp)
    STORE   x29, 28*REGLEN(sp)
    STORE   x30, 29*REGLEN(sp)
    STORE   x31, 29*REGLEN(sp)
.endm

.macro RESTORE_CONTEXT
    LOAD   x1, 0*REGLEN(sp)
    LOAD   x3, 2*REGLEN(sp)
    LOAD   x4, 3*REGLEN(sp)
    LOAD   x5, 4*REGLEN(sp)
    LOAD   x6, 5*REGLEN(sp)
    LOAD   x7, 6*REGLEN(sp)
    LOAD   x8, 7*REGLEN(sp)
    LOAD   x9, 8*REGLEN(sp)
    LOAD   x10, 9*REGLEN(sp)
    LOAD   x11, 10*REGLEN(sp)
    LOAD   x12, 11*REGLEN(sp)
    LOAD   x13, 12*REGLEN(sp)
    LOAD   x14, 13*REGLEN(sp)
    LOAD   x15, 14*REGLEN(sp)
    LOAD   x16, 15*REGLEN(sp)
    LOAD   x17, 16*REGLEN(sp)
    LOAD   x18, 17*REGLEN(sp)
    LOAD   x19, 18*REGLEN(sp)
    LOAD   x20, 19*REGLEN(sp)
    LOAD   x21, 20*REGLEN(sp)
    LOAD   x22, 21*REGLEN(sp)
    LOAD   x23, 22*REGLEN(sp)
    LOAD   x24, 23*REGLEN(sp)
    LOAD   x25, 24*REGLEN(sp)
    LOAD   x26, 25*REGLEN(sp)
    LOAD   x27, 26*REGLEN(sp)
    LOAD   x28, 27*REGLEN(sp)
    LOAD   x29, 28*REGLEN(sp)
    LOAD   x30, 29*REGLEN(sp) 
    LOAD   x31, 30*REGLEN(sp) 

    add sp, sp, (REGLEN*31) 
.endm

.balign 0x4
.global mhandler_entry
mhandler_entry:
    SAVE_CONTEXT
    jal mhandler
    j _return

.balign 0x4
.global hshandler_entry
hshandler_entry:
    SAVE_CONTEXT
    jal hshandler
    j _return

.balign 0x4
.global vshandler_entry
vshandler_entry:
    SAVE_CONTEXT
    jal vshandler
    j _return
    
_return:
    li t0, 4 //make sure PRIV_M is 4
    beq a0, t0, _return_from_m 
_return_from_s:
    RESTORE_CONTEXT
    sret
    j   .
_return_from_m:
    RESTORE_CONTEXT
    mret
