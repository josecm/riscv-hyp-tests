#include <rvh_test.h>
#include <page_tables.h>

#define TINST_LOAD(instuction) ((instruction) & 0x703f)

bool tinst_tests(){
    
    TEST_START();

    hspt_init();
    goto_priv(PRIV_HS);
    uintptr_t vaddr_f = hs_page_base(VSI_GI);
    uint32_t instruction = 0;

    TEST_SETUP_EXCEPT();
    lb(vaddr_f);
    if(excpt.triggered) instruction = *((uint16_t*)excpt.epc) | *((uint16_t*)excpt.epc + 2) << 16;
    TEST_ASSERT("correct tinst when executing a lb which results in a lpf",
        excpt.triggered == true && 
        excpt.cause == CAUSE_LPF &&
        excpt.tinst == TINST_LOAD(instruction)
    );

    TEST_END();
}