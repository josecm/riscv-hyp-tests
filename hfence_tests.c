#include <rvh_test.h>
#include <page_tables.h> 

bool hfence_test() {

    TEST_START();
    uintptr_t vaddr;
    uint64_t val;
    bool cond;
    write64(phys_page_base(SWITCH1), 0x111111111);
    write64(phys_page_base(SWITCH2), 0x222222222);

    //////////////////////////////////////////////////////////////////////

    goto_priv(PRIV_HS);
    set_prev_priv(PRIV_VS);
    hpt_init();
    vspt_init();
    vaddr = vs_page_base(SWITCH1);

    val = hlvd(vaddr);
    cond = true;
    hpt_switch();
    cond &= hlvd(vaddr) == val;
    hfence_vvma();
    cond &= hlvd(vaddr) != val;
    hpt_switch();
    cond &= hlvd(vaddr) != val;
    hfence_gvma();
    cond &= hlvd(vaddr) == val;
    TEST_ASSERT("hfences correctly invalidate guest tlb entries", cond);

    //////////////////////////////////////////////////////////////////////

    goto_priv(PRIV_HS);
    val = hlvd(vaddr);
    cond = true;
    hpt_switch();
    sfence();
    cond &= hlvd(vaddr) == val;
    TEST_ASSERT("hs sfence doest not affect guest level tlb entries", cond);

    //////////////////////////////////////////////////////////////////////  

    goto_priv(PRIV_HS);
    hspt_init();
    vaddr = hs_page_base(SWITCH1);
    val = read64(vaddr);
    cond = true;

    hspt_switch();
    goto_priv(PRIV_VS);
    sfence();
    goto_priv(PRIV_HS);
    cond &= read64(vaddr) == val;
    TEST_ASSERT("vs sfence doest not affect hypervisor level tlb entries", cond);

    //////////////////////////////////////////////////////////////////////


    
    TEST_END();
}