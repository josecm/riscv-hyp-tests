#include <rvh_test.h>
#include <page_tables.h>

static inline uint64_t lb(uintptr_t addr){
    asm volatile(
        "lb t0, 0(%0)\n\t"
        :: "r"(addr): "t0", "memory"\
    );
}

static inline uint64_t lbu(uintptr_t addr){
    asm volatile(
        "lbu t0, 0(%0)\n\t"
        :: "r"(addr): "t0", "memory"\
    );
}

static inline uint64_t lh(uintptr_t addr){
    asm volatile(
        "lh t0, 0(%0)\n\t"
        :: "r"(addr): "t0", "memory"\
    );
}

static inline uint64_t lhu(uintptr_t addr){
    asm volatile(
        "lhu t0, 0(%0)\n\t"
        :: "r"(addr): "t0", "memory"\
    );
}

static inline uint64_t lw(uintptr_t addr){
    asm volatile(
        "lw t0, 0(%0)\n\t"
        :: "r"(addr): "t0", "memory"\
    );
}

static inline uint64_t lwu(uintptr_t addr){
    asm volatile(
        "lwu t0, 0(%0)\n\t"
        :: "r"(addr): "t0", "memory"\
    );
}

static inline uint64_t ld(uintptr_t addr){
    asm volatile(
        "ld t0, 0(%0)\n\t"
        :: "r"(addr): "t0", "memory"\
    );
}

static inline uint64_t sb(uintptr_t addr){
    asm volatile(
        "sb t0, 0(%0)\n\t"
        :: "r"(addr): "t0", "memory"\
    );
}

static inline uint64_t sw(uintptr_t addr){
    asm volatile(
        "sw t0, 0(%0)\n\t"
        :: "r"(addr): "t0", "memory"\
    );
}

static inline uint64_t sh(uintptr_t addr){
    asm volatile(
        "sh t0, 0(%0)\n\t"
        :: "r"(addr): "t0", "memory"\
    );
}

static inline uint64_t sd(uintptr_t addr){
    asm volatile(
        "sd t0, 0(%0)\n\t"
        :: "r"(addr): "t0", "memory"\
    );
}

static inline uint64_t c_lw(uintptr_t addr){
    asm volatile(
        "li a0, 0\n\t"
        "add a0,x0, %0\n\t"
        "c.lw a0, 0(a0)\n\t"
        :: "r"(addr): "a0", "memory"\
    );
}

static inline uint64_t c_ld(uintptr_t addr){
    asm volatile(
        "li a0, 0\n\t"
        "add a0,x0, %0\n\t"
        "c.ld a0, 0(a0)\n\t"
        :: "r"(addr): "a0", "memory"\
    );
}

static inline uint64_t c_sw(uintptr_t addr){
    asm volatile(
        "li a0, 0\n\t"
        "add a0,x0, %0\n\t"
        "c.sw a0, 0(a0)\n\t"
        :: "r"(addr): "a0", "memory"\
    );
}

static inline uint64_t c_sd(uintptr_t addr){
    asm volatile(
        "li a0, 0\n\t"
        "add a0,x0, %0\n\t"
        "c.sd a0, 0(a0)\n\t"
        :: "r"(addr): "a0", "memory"\
    );
}

static inline uint64_t lr_w(uintptr_t addr){
    asm volatile(
        "lr.w t1, 0(%0)\n\t"
        :: "r"(addr): "t0", "memory"\
    );
}

static inline uint64_t sc_w(uintptr_t addr){
    asm volatile(
        "li t1, 0x2\n\t"
        "sc.w t1, t1, 0(%0)\n\t"
        :: "r"(addr): "t0", "t1", "memory"\
    );
}

static inline uint64_t amoswap_w(uintptr_t addr){
    asm volatile(
        "li t1, 0x2\n\t"
        "amoswap.w t1, t1, 0(%0)\n\t"
        :: "r"(addr): "t0", "t1", "memory"\
    );
}

static inline uint64_t amoadd_w(uintptr_t addr){
    asm volatile(
        "li t1, 0x2\n\t"
        "amoadd.w t1, t1, 0(%0)\n\t"
        :: "r"(addr): "t0", "t1", "memory"\
    );
}

static inline uint64_t amoxor_w(uintptr_t addr){
    asm volatile(
        "li t1, 0x2\n\t"
        "amoxor.w t1, t1, 0(%0)\n\t"
        :: "r"(addr): "t0", "t1", "memory"\
    );
}

static inline uint64_t amoand_w(uintptr_t addr){
    asm volatile(
        "li t1, 0x2\n\t"
        "amoand.w t1, t1, 0(%0)\n\t"
        :: "r"(addr): "t0", "t1", "memory"\
    );
}

static inline uint64_t amoor_w(uintptr_t addr){
    asm volatile(
        "li t1, 0x2\n\t"
        "amoor.w t1, t1, 0(%0)\n\t"
        :: "r"(addr): "t0", "t1", "memory"\
    );
}

static inline uint64_t amomin_w(uintptr_t addr){
    asm volatile(
        "li t1, 0x2\n\t"
        "amomin.w t1, t1, 0(%0)\n\t"
        :: "r"(addr): "t0", "t1", "memory"\
    );
}

static inline uint64_t amomax_w(uintptr_t addr){
    asm volatile(
        "li t1, 0x2\n\t"
        "amomax.w t1, t1, 0(%0)\n\t"
        :: "r"(addr): "t0", "t1", "memory"\
    );
}

static inline uint64_t amominu_w(uintptr_t addr){
    asm volatile(
        "li t1, 0x2\n\t"
        "amominu.w t1, t1, 0(%0)\n\t"
        :: "r"(addr): "t0", "t1", "memory"\
    );
}

static inline uint64_t amomaxu_w(uintptr_t addr){
    asm volatile(
        "li t1, 0x2\n\t"
        "amomaxu.w t1, t1, 0(%0)\n\t"
        :: "r"(addr): "t0", "t1", "memory"\
    );
}

static inline uint64_t amoswap_d(uintptr_t addr){
    asm volatile(
        "li t1, 0x2\n\t"
        "amoswap.d t1, t1, 0(%0)\n\t"
        :: "r"(addr): "t0", "t1", "memory"\
    );
}

static inline uint64_t amosadd_d(uintptr_t addr){
    asm volatile(
        "li t1, 0x2\n\t"
        "amoadd.d t1, t1, 0(%0)\n\t"
        :: "r"(addr): "t0", "t1", "memory"\
    );
}

static inline uint64_t amoxor_d(uintptr_t addr){
    asm volatile(
        "li t1, 0x2\n\t"
        "amoxor.d t1, t1, 0(%0)\n\t"
        :: "r"(addr): "t0", "t1", "memory"\
    );
}

static inline uint64_t amoand_d(uintptr_t addr){
    asm volatile(
        "li t1, 0x2\n\t"
        "amoand.d t1, t1, 0(%0)\n\t"
        :: "r"(addr): "t0", "t1", "memory"\
    );
}

static inline uint64_t amoor_d(uintptr_t addr){
    asm volatile(
        "li t1, 0x2\n\t"
        "amoor.d t1, t1, 0(%0)\n\t"
        :: "r"(addr): "t0", "t1", "memory"\
    );
}

static inline uint64_t amomin_d(uintptr_t addr){
    asm volatile(
        "li t1, 0x2\n\t"
        "amomin.d t1, t1, 0(%0)\n\t"
        :: "r"(addr): "t0", "t1", "memory"\
    );
}

static inline uint64_t amomax_d(uintptr_t addr){
    asm volatile(
        "li t1, 0x2\n\t"
        "amomax.d t1, t1, 0(%0)\n\t"
        :: "r"(addr): "t0", "t1", "memory"\
    );
}

static inline uint64_t amominu_d(uintptr_t addr){
    asm volatile(
        "li t1, 0x2\n\t"
        "amominu.d t1, t1, 0(%0)\n\t"
        :: "r"(addr): "t0", "t1", "memory"\
    );
}

static inline uint64_t amomaxu_d(uintptr_t addr){
    asm volatile(
        "li t1, 0x2\n\t"
        "amomaxu.d t1, t1, 0(%0)\n\t"
        :: "r"(addr): "t0", "t1", "memory"\
    );
}

bool tinst_tests(){
    
    TEST_START();
    hspt_init();
    goto_priv(PRIV_HS);
    uintptr_t vaddr_f;
    /* Standard Load Instructions */
    /* Instruction LB */
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSX_GUR);
    lb(vaddr_f);
    TEST_ASSERT("tinst when executing a lb causes a lpf and tinst to 0x283",
        excpt.triggered == true && 
        excpt.cause == CAUSE_LPF &&
        excpt.tinst == 0x283 &&
        excpt.xpv == false
    );

    /* Instruction LBU */
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSX_GUR);
    lbu(vaddr_f);
    TEST_ASSERT("tinst when executing a lbu causes a lpf and tinst to 0x4283",
        excpt.triggered == true && 
        excpt.cause == CAUSE_LPF &&
        excpt.tinst == 0x4283 &&
        excpt.xpv == false
    );

    /* Instruction LH */
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSX_GUR);
    lh(vaddr_f);
    TEST_ASSERT("tinst when executing a lh causes a lpf and tinst to 0x1283",
        excpt.triggered == true && 
        excpt.cause == CAUSE_LPF &&
        excpt.tinst == 0x1283 &&
        excpt.xpv == false
    );

    /* Instruction LHU */
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSX_GUR);
    lhu(vaddr_f);
    TEST_ASSERT("tinst when executing a lhu causes a lpf and tinst to 0x5283",
        excpt.triggered == true && 
        excpt.cause == CAUSE_LPF &&
        excpt.tinst == 0x5283 &&
        excpt.xpv == false
    );

    /* Instruction LW */
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSX_GUR);
    lw(vaddr_f);
    TEST_ASSERT("tinst when executing a lw causes a lpf and tinst to 0x2283",
        excpt.triggered == true && 
        excpt.cause == CAUSE_LPF &&
        excpt.tinst == 0x2283 &&
        excpt.xpv == false
    );

    /* Instruction LWU */
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSX_GUR);
    lwu(vaddr_f);
    TEST_ASSERT("tinst when executing a lwu causes a lpf and tinst to 0x6283",
        excpt.triggered == true && 
        excpt.cause == CAUSE_LPF &&
        excpt.tinst == 0x6283 &&
        excpt.xpv == false
    );

    /* Instruction LD */
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSX_GUR);
    ld(vaddr_f);
    TEST_ASSERT("tinst when executing a ld causes a lpf and tinst to 0x3283",
        excpt.triggered == true && 
        excpt.cause == CAUSE_LPF &&
        excpt.tinst == 0x3283 &&
        excpt.xpv == false
    );

    /* Instruction FLW */

    /* Instruction FLD */
    /* Standard Store Instructions */
    /* Instruction SB */
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSX_GUR);
    sb(vaddr_f);
    TEST_ASSERT("tinst when executing a sb causes a spf and tinst to 0x500023",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == 0x500023 &&
        excpt.xpv == false
    );
    /* Instruction SH */
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSX_GUR);
    sh(vaddr_f);
    TEST_ASSERT("tinst when executing a sh causes a spf and tinst to 0x501023",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == 0x501023 &&
        excpt.xpv == false
    );
    /* Instruction SW */
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSX_GUR);
    sw(vaddr_f);
    TEST_ASSERT("tinst when executing a sw causes a spf and tinst to 0x502023",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == 0x502023 &&
        excpt.xpv == false
    );
    /* Instruction SD */
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSX_GUR);
    sd(vaddr_f);
    TEST_ASSERT("tinst when executing a sd causes a spf and tinst to 0x506023",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == 0x506023 &&
        excpt.xpv == false
    );
    /* Instruction FSW */
    /* Instruction FSD */
    /* Instruction FSQ */
    /* Compressed Instructions */
    /* Instruction C.LW */
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSR_GUR);
    c_lw(vaddr_f);
    TEST_ASSERT("tinst when executing a c_lw causes a lpf and tinst to 0x2281",
        excpt.triggered == true && 
        excpt.cause == CAUSE_LPF &&
        excpt.tinst == 0x2281 &&
        excpt.xpv == false
    );
    /* Instruction C.LD */
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSR_GUR);
    c_ld(vaddr_f);
    TEST_ASSERT("tinst when executing a c_ld causes a lpf and tinst to 0x502022",
        excpt.triggered == true && 
        excpt.cause == CAUSE_LPF &&
        excpt.tinst == 0x502022 &&
        excpt.xpv == false
    );
    /* Instruction C.SW */
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSR_GUR);
    c_sw(vaddr_f);
    TEST_ASSERT("tinst when executing a c_sw causes a spf and tinst to 0x506021",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == 0x506021 &&
        excpt.xpv == false
    );
    /* Instruction C.SD */
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSR_GUR);
    c_sd(vaddr_f);
    TEST_ASSERT("tinst when executing a c_sd causes a spf and tinst to 0x3283",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == 0x3283 &&
        excpt.xpv == false
    );
    /* Standard Atomic Instructions */
    /* Instruction LR.W */
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSR_GUR);
    lr_w(vaddr_f);
    TEST_ASSERT("tinst when executing a lr_w causes a spf and tinst to 0x11000232F",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == 0x1000232F &&
        excpt.xpv == false
    );
    /* Instruction SC.W */
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSR_GUR);
    sc_w(vaddr_f);
    TEST_ASSERT("tinst when executing a sc_w causes a spf and tinst to 0x1860232F",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == 0x1860232F &&
        excpt.xpv == false
    );
    /* Instruction AMOSWAP.W */
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSR_GUR);
    amoswap_w(vaddr_f);
    TEST_ASSERT("tinst when executing a amoswap_w causes a spf and tinst to 0x860232f",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == 0x860232f &&
        excpt.xpv == false
    );
    /* Instruction AMOADD.W  */
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSR_GUR);
    amoadd_w(vaddr_f);
    TEST_ASSERT("tinst when executing a amoadd_w causes a spf and tinst to 0x60232F",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == 0x60232F &&
        excpt.xpv == false
    );
    /* Instruction AMOXOR.W  */
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSR_GUR);
    amoxor_w(vaddr_f);
    TEST_ASSERT("tinst when executing a amoxor_w causes a spf and tinst to 0x2060232F",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == 0x2060232F &&
        excpt.xpv == false
    );
    /* Instruction AMOAND.W  */
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSR_GUR);
    amoand_w(vaddr_f);
    TEST_ASSERT("tinst when executing a amoand_w causes a spf and tinst to 0x6060232F",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == 0x6060232F &&
        excpt.xpv == false
    );
    /* Instruction AMOOR.W   */
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSR_GUR);
    amoor_w(vaddr_f);
    TEST_ASSERT("tinst when executing a amoor_w causes a spf and tinst to 0x4060232F",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == 0x4060232F &&
        excpt.xpv == false
    );
    /* Instruction AMOMIN.W  */
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSR_GUR);
    amomin_w(vaddr_f);
    TEST_ASSERT("tinst when executing a amomin_w causes a spf and tinst to 0x8060232F",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == 0x8060232F &&
        excpt.xpv == false
    );
    /* Instruction AMOMAX.W  */
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSR_GUR);
    amomax_w(vaddr_f);
    TEST_ASSERT("tinst when executing a amomax_w causes a spf and tinst to 0xA060232F",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == 0xA060232F &&
        excpt.xpv == false
    );
    /* Instruction AMOMINU.W */
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSR_GUR);
    amominu_w(vaddr_f);
    TEST_ASSERT("tinst when executing a amominu_w causes a spf and tinst to 0xC060232F",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == 0xC060232F &&
        excpt.xpv == false
    );
    /* Instruction AMOMAXU.W */
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSR_GUR);
    amomaxu_w(vaddr_f);
    TEST_ASSERT("tinst when executing a amomaxu_w causes a spf and tinst to 0xE060232F",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == 0xE060232F &&
        excpt.xpv == false
    );
    /* Instruction AMOSWAP.D */
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSR_GUR);
    amoswap_w(vaddr_f);
    TEST_ASSERT("tinst when executing a amoswap_w causes a spf and tinst to 0x860232f",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == 0x860232f &&
        excpt.xpv == false
    );
    /* Instruction AMOADD.D  */
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSR_GUR);
    amoadd_w(vaddr_f);
    TEST_ASSERT("tinst when executing a amoadd_w causes a spf and tinst to 0x60232F",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == 0x60232F &&
        excpt.xpv == false
    );
    /* Instruction AMOXOR.D  */
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSR_GUR);
    amoxor_w(vaddr_f);
    TEST_ASSERT("tinst when executing a amoxor_w causes a spf and tinst to 0x2060232F",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == 0x2060232F &&
        excpt.xpv == false
    );
    /* Instruction AMOAND.D  */
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSR_GUR);
    amoand_w(vaddr_f);
    TEST_ASSERT("tinst when executing a amoand_w causes a spf and tinst to 0x6060232F",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == 0x6060232F &&
        excpt.xpv == false
    );
    /* Instruction AMOOR.D   */
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSR_GUR);
    amoor_w(vaddr_f);
    TEST_ASSERT("tinst when executing a amoor_w causes a spf and tinst to 0x4060232F",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == 0x4060232F &&
        excpt.xpv == false
    );
    /* Instruction AMOMIN.D  */
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSR_GUR);
    amomin_w(vaddr_f);
    TEST_ASSERT("tinst when executing a amomin_w causes a spf and tinst to 0x8060232F",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == 0x8060232F &&
        excpt.xpv == false
    );
    /* Instruction AMOMAX.D  */
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSR_GUR);
    amomax_w(vaddr_f);
    TEST_ASSERT("tinst when executing a amomax_w causes a spf and tinst to 0xA060232F",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == 0xA060232F &&
        excpt.xpv == false
    );
    /* Instruction AMOMINU.D */
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSR_GUR);
    amominu_w(vaddr_f);
    TEST_ASSERT("tinst when executing a amominu_w causes a spf and tinst to 0xC060232F",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == 0xC060232F &&
        excpt.xpv == false
    );
    /* Instruction AMOMAXU.D */
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSR_GUR);
    amomaxu_w(vaddr_f);
    TEST_ASSERT("tinst when executing a amomaxu_w causes a spf and tinst to 0xE060232F",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == 0xE060232F &&
        excpt.xpv == false
    );

    TEST_END();
}