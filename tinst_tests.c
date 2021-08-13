#include <rvh_test.h>
#include <page_tables.h>

#define TINST_LOAD(instuction) ((instruction) & 0x7fff)

bool tinst_tests(){
    
    TEST_START();

    hspt_init();
    goto_priv(PRIV_HS);
    uintptr_t vaddr_f = hs_page_base(VSI_GI);
    uint32_t instruction = 0;
    uint64_t value = 0xdeadbeef;

    TEST_SETUP_EXCEPT();
    value = lb(vaddr_f);
    if(excpt.triggered) instruction = *((uint16_t*)excpt.epc) | *((uint16_t*)excpt.epc + 2) << 16;
    TEST_ASSERT("correct tinst when executing a lb which results in a lpf",
        excpt.triggered == true && 
        excpt.cause == CAUSE_LPF &&
        excpt.tinst == TINST_LOAD(instruction)
    );

    TEST_SETUP_EXCEPT();
    value = lbu(vaddr_f);
    if(excpt.triggered) instruction = *((uint16_t*)excpt.epc) | *((uint16_t*)excpt.epc + 2) << 16;
    TEST_ASSERT("correct tinst when executing a lbu which results in a lpf",
        excpt.triggered == true && 
        excpt.cause == CAUSE_LPF &&
        excpt.tinst == TINST_LOAD(instruction)
    );

    TEST_SETUP_EXCEPT();
    value = lh(vaddr_f);
    if(excpt.triggered) instruction = *((uint16_t*)excpt.epc) | *((uint16_t*)excpt.epc + 2) << 16;
    TEST_ASSERT("correct tinst when executing a lh which results in a lpf",
        excpt.triggered == true && 
        excpt.cause == CAUSE_LPF &&
        excpt.tinst == TINST_LOAD(instruction)
    );

    TEST_SETUP_EXCEPT();
    value = lhu(vaddr_f);
    if(excpt.triggered) instruction = *((uint16_t*)excpt.epc) | *((uint16_t*)excpt.epc + 2) << 16;
    TEST_ASSERT("correct tinst when executing a lhu which results in a lpf",
        excpt.triggered == true && 
        excpt.cause == CAUSE_LPF &&
        excpt.tinst == TINST_LOAD(instruction)
    );

    TEST_SETUP_EXCEPT();
    lw(vaddr_f);
    if(excpt.triggered) instruction = *((uint16_t*)excpt.epc) | *((uint16_t*)excpt.epc + 2) << 16;
    printf("instruction %x \r\n", instruction);
    TEST_ASSERT("correct tinst when executing a lw which results in a lpf",
        excpt.triggered == true && 
        excpt.cause == CAUSE_LPF &&
        excpt.tinst == TINST_LOAD(instruction)
    );

    TEST_SETUP_EXCEPT();
    value = lwu(vaddr_f);
    if(excpt.triggered) instruction = *((uint16_t*)excpt.epc) | *((uint16_t*)excpt.epc + 2) << 16;
    TEST_ASSERT("correct tinst when executing a lwu which results in a lpf",
        excpt.triggered == true && 
        excpt.cause == CAUSE_LPF &&
        excpt.tinst == TINST_LOAD(instruction)
    );

    TEST_SETUP_EXCEPT();
    value = ld(vaddr_f);
    if(excpt.triggered) instruction = *((uint16_t*)excpt.epc) | *((uint16_t*)excpt.epc + 2) << 16;
    printf("instruction %x \r\n", instruction);
    TEST_ASSERT("correct tinst when executing a ld which results in a lpf",
        excpt.triggered == true && 
        excpt.cause == CAUSE_LPF &&
        excpt.tinst == TINST_LOAD(instruction)
    );
    TEST_END();
}