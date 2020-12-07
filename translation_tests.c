#include <rvh_test.h>
#include <page_tables.h> 

static inline void touchread(uintptr_t addr){
    asm volatile("" ::: "memory");
    volatile uint64_t x = *(volatile uint64_t *)addr;
}

static inline void touchwrite(uintptr_t addr){
    *(volatile uint64_t *)addr = 0xdeadbeef;
}

static inline void touch(uintptr_t addr){
    touchwrite(addr);
}

bool two_stage_translation(){
    
    TEST_START();

    uintptr_t addr1 = phys_page_base(SWITCH1);
    uintptr_t addr2 = phys_page_base(SWITCH2);
    uintptr_t vaddr1 = vs_page_base(SWITCH1);
    uintptr_t vaddr2 = vs_page_base(SWITCH2);
    write64(addr1, 0x11);
    write64(addr2, 0x22);

    /**
     * Setup hyp page_tables.
     */
    goto_priv(PRIV_HS);
    hspt_init();
    hpt_init();

    /**
     * Setup guest page tables.
     */
    goto_priv(PRIV_VS);
    vspt_init();

    bool check1 = read64(vaddr1) == 0x11;
    bool check2 = read64(vaddr2) == 0x22;
    TEST_ASSERT("vs gets right values", check1 && check2);
    
    goto_priv(PRIV_HS);
    hpt_switch();
    hfence();
    goto_priv(PRIV_VS);
    check1 = read64(vaddr1) == 0x22;
    check2 = read64(vaddr2) == 0x11;   
    // INFO("0%lx 0x%lx", read64(vaddr1), read64(vaddr2));
    TEST_ASSERT("vs gets right values after changing 2nd stage pt", check1 && check2);

    vspt_switch();
    sfence();
    check1 = read64(vaddr1) == 0x11;
    check2 = read64(vaddr2) == 0x22;   
    TEST_ASSERT("vs gets right values after changing 1st stage pt", check1 && check2);

    goto_priv(PRIV_M); 
    CSRS(medeleg, 1ull << CAUSE_LGPF);
    goto_priv(PRIV_VS);
    TEST_SETUP_EXCEPT();
    read64(vs_page_base(VSRWX_GI));    
    TEST_ASSERT(
        "load guest page fault on unmapped address", 
        excpt.triggered == true && 
        excpt.cause == CAUSE_LGPF &&
        excpt.tval2 == (vs_page_base(VSRWX_GI) >> 2) &&
        excpt.priv == PRIV_HS &&
        excpt.gva == true &&
        excpt.xpv == true
    );

    TEST_SETUP_EXCEPT();
    TEST_EXEC_EXCEPT(vs_page_base(VSRWX_GI)); 
    TEST_ASSERT(
        "instruction guest page fault on unmapped 2-stage address", 
        excpt.triggered == true && 
        excpt.cause == CAUSE_IGPF &&
        excpt.tval2 == (vs_page_base(VSRWX_GI) >> 2) &&
        excpt.priv == PRIV_M  &&
        excpt.gva == true &&
        excpt.xpv == true
    );

    goto_priv(PRIV_M);
    CSRS(medeleg, 1 << CAUSE_LPF | 1 << CAUSE_LGPF);
    goto_priv(PRIV_HS); 
    CSRS(CSR_HEDELEG, 1 << CAUSE_LPF);
    goto_priv(PRIV_VS);
    sfence();
    TEST_SETUP_EXCEPT();
    touchread(vs_page_base(VSI_GI));    
    TEST_ASSERT(
        "invalid pte in both stages leads to s1 page fault", 
        excpt.triggered == true && 
        excpt.cause == CAUSE_LPF &&
        excpt.priv == PRIV_VS && 
        excpt.gva == false
    );

    TEST_END();
}

bool second_stage_only_translation(){

    /**
     * Test only 2nd stage translation.
     */
    TEST_START();

    uintptr_t addr1 = phys_page_base(SWITCH1);
    uintptr_t addr2 = phys_page_base(SWITCH2);
    uintptr_t vaddr1 = vs_page_base(SWITCH1);
    uintptr_t vaddr2 = vs_page_base(SWITCH2);
    write64(addr1, 0x11);
    write64(addr2, 0x22);   


    CSRS(medeleg, (1 << CAUSE_LGPF) | (1 << CAUSE_SGPF));

    /**
     * Setup hyp page_tables.
     */
    goto_priv(PRIV_HS);
    hspt_init();
    hpt_init();
    
    goto_priv(PRIV_VS);
    TEST_SETUP_EXCEPT();
    bool check1 = read64(vaddr1) == 0x11;
    bool check2 = read64(vaddr2) == 0x22;
    TEST_ASSERT("vs gets right values", excpt.triggered == false && check1 && check2);

    hpt_switch();
    sfence();
    TEST_SETUP_EXCEPT();
    check1 = read64(vaddr1) == 0x22;
    check2 = read64(vaddr2) == 0x11;   
    TEST_ASSERT("vs gets right values after changing pt", excpt.triggered == false && check1 && check2);

    TEST_SETUP_EXCEPT();
    (void) read64(vs_page_base(VSRWX_GI));  
    TEST_ASSERT(
        "vs access to unmapped -> load gpf",
        excpt.triggered == true && 
        excpt.cause == CAUSE_LGPF &&
        excpt.priv == PRIV_HS
    );

    TEST_SETUP_EXCEPT();
    read64(vs_page_base_limit(TOP));
    TEST_ASSERT(
        "access top of guest pa space with high bits == 0", 
        excpt.triggered == false
    );
    
    TEST_SETUP_EXCEPT();
    read64(vs_page_base_limit(TOP) | (1ULL << 41));
    TEST_ASSERT(
        "access top of guest pa space with high bits =/= 0", 
        excpt.triggered == true &&
        excpt.cause == CAUSE_LGPF
    );

    TEST_END();
}

static inline uint64_t read64_mprv(unsigned priv, uintptr_t addr){

    if(curr_priv != PRIV_M){
        ERROR("trying to read as mprv from low privilege");
    }

    uint64_t value = 0;
    set_prev_priv(priv);
    asm volatile(
        "csrs   mstatus, %2\n\t"
        "ld  %0, %1\n\t" 
        "csrc   mstatus, %2\n\t"
        : "=&r"(value) : "m"(*(uint64_t*)addr), "r"(MSTATUS_MPRV)
    );

    return value;
}


static inline void write64_mprv(unsigned priv, uintptr_t addr, uint64_t value){

    if(curr_priv != PRIV_M){
        ERROR("trying to write as mprv from low privilege");
    }

    set_prev_priv(priv);
    asm volatile(
        "csrs   mstatus, %2\n\t"
        "sd  %0, %1\n\t" 
        "csrc   mstatus, %2\n\t"
        :: "r"(value), "m"(*(uint64_t*)addr), "r"(MSTATUS_MPRV)
    );

}


bool m_and_hs_using_vs_access(){

    uint64_t val, valu;
    uintptr_t vaddr = vs_page_base(SCRATCHPAD);
    uintptr_t addr;

    TEST_START();
    
    hspt_init();
    hpt_init();
    vspt_init();

    TEST_SETUP_EXCEPT();
    write64_mprv(PRIV_VS, vaddr, 0x1107ec0ffee);
    val = read64_mprv(PRIV_VS, vaddr);
    TEST_ASSERT("machine sets mprv to access vs space",
        excpt.triggered == false && val == 0x1107ec0ffee
    );

    //TODO: test mprv to access hs space

    goto_priv(PRIV_HS);
    set_prev_priv(PRIV_VS);

    TEST_SETUP_EXCEPT();
    hsvd(vaddr, 0xdeadbeef);
    val = hlvd(vaddr);
    TEST_ASSERT("hs hlvd",
        excpt.triggered == false && val == 0xdeadbeef
    );

    TEST_SETUP_EXCEPT();
    hsvb(vaddr, ((uint8_t)-1));
    val = hlvb(vaddr);
    valu = hlvbu(vaddr);
    TEST_ASSERT("hs hlvb vs hlvbu",
        excpt.triggered == false && val == (-1) && valu == ((uint8_t)-1)
    );

    TEST_SETUP_EXCEPT();
    hsvh(vaddr, ((uint16_t)-1));
    val = hlvh(vaddr);
    valu = hlvhu(vaddr);
    TEST_ASSERT("hs hlvh vs hlvhu",
        excpt.triggered == false && val == (-1) && valu == ((uint16_t)-1)
    );

    TEST_SETUP_EXCEPT();
    hsvw(vaddr, ((uint32_t)-1));
    val = hlvw(vaddr);
    valu = hlvwu(vaddr);
    printf("val 0x%llx, valu 0x%llx\n", val, valu);
    TEST_ASSERT("hs hlvw vs hlvwu",
        excpt.triggered == false && val == (-1) && valu == ((uint32_t)-1)
    );

    /**
     * Some tests are commented out because qemu behaves in a weird way if you
     * try to use a hlvx instruction on a page without read permissions. It
     * gives you a load access fault, with the mepc not even pointing to the
     * hlvx instruction but on a previous lui instruction. 
     * TODO: find out why
     */

    TEST_SETUP_EXCEPT();
    val = hlvxwu(vs_page_base(VSX_GUX));
    TEST_ASSERT("hs hlvxwu accesses on only execute page",
        excpt.triggered == false
    );

    TEST_SETUP_EXCEPT();
    val = hlvxwu(vs_page_base(VSRWX_GURWX));
    TEST_ASSERT("hs hlvxwu accesses page with all permissions",
        excpt.triggered == false
    );

    TEST_SETUP_EXCEPT();
    val = hlvxwu(vs_page_base(VSRWX_GURW));
    TEST_ASSERT("hs hlvxwu on hs-level non-exec page leads to lgpf",
        excpt.triggered == true && 
        excpt.cause == CAUSE_LGPF && 
        excpt.gva == true &&
        excpt.xpv == false
    );

    TEST_SETUP_EXCEPT();
    val = hlvxwu(vs_page_base(VSRW_GURWX));
    TEST_ASSERT("hs hlvxwu on vs-level non-exec page leads to lpf",
        excpt.triggered == true && 
        excpt.cause == CAUSE_LPF  && 
        excpt.gva == true &&
        excpt.xpv == false
    );

    vaddr = vs_page_base(VSURWX_GURWX);
    addr = phys_page_base(VSURWX_GURWX);

    goto_priv(PRIV_M);

    TEST_SETUP_EXCEPT();
    write64(addr, 0x1107ec0ffee);
    val = read64_mprv(PRIV_VS, vaddr);
    TEST_ASSERT("machine mprv vs access to vu leads to exception",
        excpt.triggered == true
    );

    TEST_SETUP_EXCEPT();
    write64(addr, 0x1107ec0ffee);
    val = read64_mprv(PRIV_VU, vaddr);
    TEST_ASSERT("machine mprv vu access to vu successful",
        excpt.triggered == false
    );

    goto_priv(PRIV_HS);

    set_prev_priv(PRIV_VU);
    TEST_SETUP_EXCEPT();
    write64(addr, 0x1107ec0ffee);
    val = hlvd(vaddr);
    TEST_ASSERT("hs hlvd to vu page successful when spvp = 0",
        excpt.triggered == false
    );

    set_prev_priv(PRIV_VS);
    TEST_SETUP_EXCEPT();
    write64(addr, 0x1107ec0ffee);
    val = hlvd(vaddr);
    TEST_ASSERT("hs hlvd to vu page leads to exception when spvp = 1",
        excpt.triggered == true
    );

    CSRS(CSR_VSSTATUS, SSTATUS_SUM);
    goto_priv(PRIV_M);

    TEST_SETUP_EXCEPT();
    write64(addr, 0x1107ec0ffee);
    val = read64_mprv(PRIV_VS, vaddr);
    TEST_ASSERT("machine mprv access vs user page successful when vsstatus.sum set",
        excpt.triggered == false && val == 0x1107ec0ffee
    );

    goto_priv(PRIV_HS);
    set_prev_priv(PRIV_VS);
    TEST_SETUP_EXCEPT();
    write64(addr, 0x1107ec0ffee);
    val = hlvd(vaddr);
    TEST_ASSERT("hs hlvd to user page successful when vsstatus.sum set",
        excpt.triggered == false && val == 0x1107ec0ffee
    );
    CSRC(CSR_VSSTATUS, SSTATUS_SUM);

    vaddr = vs_page_base(VSX_GUX);
    addr = phys_page_base(VSX_GUX);
    goto_priv(PRIV_HS);
    set_prev_priv(PRIV_VS);
    TEST_SETUP_EXCEPT();
    val = hlvd(vaddr);
    TEST_ASSERT("hs hlvd of xo vs page leads to exception",
        excpt.triggered == true
    );
    TEST_SETUP_EXCEPT();
    CSRS(sstatus, SSTATUS_MXR);
    val = hlvd(vaddr);
    TEST_ASSERT("hs hlvd of xo vs page succsseful",
        excpt.triggered == false
    );
    CSRC(sstatus, SSTATUS_MXR);

    vaddr = vs_page_base(VSX_GUR);
    addr = phys_page_base(VSX_GUR);
    goto_priv(PRIV_HS);
    set_prev_priv(PRIV_VS);
    CSRW(sscratch, 0x1111111);
    TEST_SETUP_EXCEPT();
    val = hlvd(vaddr);
    TEST_ASSERT("hs hlvd of xo vs page leads to load page fault",
        excpt.triggered == true &&
        excpt.cause == CAUSE_LPF
    );
    TEST_SETUP_EXCEPT();
    CSRS(CSR_VSSTATUS, SSTATUS_MXR);
    val = hlvd(vaddr);
    TEST_ASSERT("hs hlvd of xo vs page succsseful with sstatus.mxr set",
        excpt.triggered == false
    );

    reset_state();
    goto_priv(PRIV_HS);
    set_prev_priv(PRIV_VS);
    hpt_init();
    
    vaddr = vs_page_base(VSI_GUR) + 1;
    TEST_SETUP_EXCEPT();\
    hsvb(vaddr, 0xdeadbeef);
    TEST_ASSERT("hs hsvb on ro 2-stage page leads to store guest page fault",
        excpt.triggered == true &&  
        excpt.cause == CAUSE_SGPF &&
        excpt.tval2 == vaddr >> 2
    );

    vaddr = vs_page_base(VSI_GUR);
    INFO_PRINT(vaddr);
    TEST_SETUP_EXCEPT();
    val = hlvb(vaddr);
    TEST_ASSERT("hs hlvb on ro 2-stage page successfull",
        excpt.triggered == false
    );

    vspt_init();
    vaddr = vs_page_base(VSR_GUR);
    TEST_SETUP_EXCEPT();
    CSRW(sscratch, 0x911);
    hsvb(vaddr, 0xdeadbeef);
    TEST_ASSERT("hs hsvb on ro both stage page leads to store page fault",
        excpt.triggered == true &&  
        excpt.cause == CAUSE_SPF
    );

    vaddr = vs_page_base(VSRW_GI);
    TEST_SETUP_EXCEPT();
    CSRW(sscratch, 0x112);
    hsvb(vaddr, 0xdeadbeef);
    TEST_ASSERT("hs hsvb on invalid 2 stage page leads to store guest page fault",
        excpt.triggered == true &&  
        excpt.cause == CAUSE_SGPF
    );

    TEST_END();

}
