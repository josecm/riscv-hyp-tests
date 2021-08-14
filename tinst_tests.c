#include <rvh_test.h>
#include <page_tables.h>

#define TINST_LOAD(instuction) ((instruction) & 0x7fff)
#define TINST_STORE(instuction) ((instruction) & 0x1f0707f)
#define TINST_AMO(instuction) ((instruction) & 0xfff07fff)

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
    TEST_ASSERT("correct tinst when executing a ld which results in a lpf",
        excpt.triggered == true && 
        excpt.cause == CAUSE_LPF &&
        excpt.tinst == TINST_LOAD(instruction)
    );

    value = 0xdeadbeef;

    TEST_SETUP_EXCEPT();
    sb(vaddr_f,value);
    if(excpt.triggered) instruction = *((uint16_t*)excpt.epc) | *((uint16_t*)excpt.epc + 2) << 16;
    TEST_ASSERT("correct tinst when executing a sb which results in a spf",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == TINST_STORE(instruction)
    );

    TEST_SETUP_EXCEPT();
    sh(vaddr_f,value);
    if(excpt.triggered) instruction = *((uint16_t*)excpt.epc) | *((uint16_t*)excpt.epc + 2) << 16;
    TEST_ASSERT("correct tinst when executing a sh which results in a spf",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == TINST_STORE(instruction)
    );

    TEST_SETUP_EXCEPT();
    sw(vaddr_f,value);
    if(excpt.triggered) instruction = *((uint16_t*)excpt.epc) | *((uint16_t*)excpt.epc + 2) << 16;
    TEST_ASSERT("correct tinst when executing a sw which results in a spf",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == TINST_STORE(instruction)
    );

    TEST_SETUP_EXCEPT();
    sd(vaddr_f,value);
    if(excpt.triggered) instruction = *((uint16_t*)excpt.epc) | *((uint16_t*)excpt.epc + 2) << 16;
    TEST_ASSERT("correct tinst when executing a sd which results in a spf",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == TINST_STORE(instruction)
    );

    TEST_SETUP_EXCEPT();
    value = amoswap_w(vaddr_f,value);
    if(excpt.triggered) instruction = *((uint16_t*)excpt.epc) | *((uint16_t*)excpt.epc + 2) << 16;
    TEST_ASSERT("correct tinst when executing a amoswap.w which results in a spf",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == TINST_AMO(instruction)
    );

    TEST_SETUP_EXCEPT();
    value = amoadd_w(vaddr_f,value);
    if(excpt.triggered) instruction = *((uint16_t*)excpt.epc) | *((uint16_t*)excpt.epc + 2) << 16;
    TEST_ASSERT("correct tinst when executing a amoadd.w which results in a spf",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == TINST_AMO(instruction)
    );

    TEST_SETUP_EXCEPT();
    value = amoxor_w(vaddr_f,value);
    if(excpt.triggered) instruction = *((uint16_t*)excpt.epc) | *((uint16_t*)excpt.epc + 2) << 16;
    TEST_ASSERT("correct tinst when executing a amoxor.w which results in a spf",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == TINST_AMO(instruction)
    );

    TEST_SETUP_EXCEPT();
    value = amoand_w(vaddr_f,value);
    if(excpt.triggered) instruction = *((uint16_t*)excpt.epc) | *((uint16_t*)excpt.epc + 2) << 16;
    TEST_ASSERT("correct tinst when executing a amoand.w which results in a spf",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == TINST_AMO(instruction)
    );

    TEST_SETUP_EXCEPT();
    value = amoor_w(vaddr_f,value);
    if(excpt.triggered) instruction = *((uint16_t*)excpt.epc) | *((uint16_t*)excpt.epc + 2) << 16;
    TEST_ASSERT("correct tinst when executing a amoor.w which results in a spf",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == TINST_AMO(instruction)
    );

    TEST_SETUP_EXCEPT();
    value = amomin_w(vaddr_f,value);
    if(excpt.triggered) instruction = *((uint16_t*)excpt.epc) | *((uint16_t*)excpt.epc + 2) << 16;
    TEST_ASSERT("correct tinst when executing a amomin.w which results in a spf",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == TINST_AMO(instruction)
    );

    TEST_SETUP_EXCEPT();
    value = amomax_w(vaddr_f,value);
    if(excpt.triggered) instruction = *((uint16_t*)excpt.epc) | *((uint16_t*)excpt.epc + 2) << 16;
    TEST_ASSERT("correct tinst when executing a amomax.w which results in a spf",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == TINST_AMO(instruction)
    );

    TEST_SETUP_EXCEPT();
    value = amominu_w(vaddr_f,value);
    if(excpt.triggered) instruction = *((uint16_t*)excpt.epc) | *((uint16_t*)excpt.epc + 2) << 16;
    TEST_ASSERT("correct tinst when executing a amominu.w which results in a spf",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == TINST_AMO(instruction)
    );

    TEST_SETUP_EXCEPT();
    value = amomaxu_w(vaddr_f,value);
    if(excpt.triggered) instruction = *((uint16_t*)excpt.epc) | *((uint16_t*)excpt.epc + 2) << 16;
    TEST_ASSERT("correct tinst when executing a amomaxu.w which results in a spf",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == TINST_AMO(instruction)
    );

    TEST_SETUP_EXCEPT();
    value = amoswap_d(vaddr_f,value);
    if(excpt.triggered) instruction = *((uint16_t*)excpt.epc) | *((uint16_t*)excpt.epc + 2) << 16;
    TEST_ASSERT("correct tinst when executing a amoswap.d which results in a spf",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == TINST_AMO(instruction)
    );

    TEST_SETUP_EXCEPT();
    value = amoadd_d(vaddr_f,value);
    if(excpt.triggered) instruction = *((uint16_t*)excpt.epc) | *((uint16_t*)excpt.epc + 2) << 16;
    TEST_ASSERT("correct tinst when executing a amoadd.d which results in a spf",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == TINST_AMO(instruction)
    );

    TEST_SETUP_EXCEPT();
    value = amoxor_d(vaddr_f,value);
    if(excpt.triggered) instruction = *((uint16_t*)excpt.epc) | *((uint16_t*)excpt.epc + 2) << 16;
    TEST_ASSERT("correct tinst when executing a amoxor.d which results in a spf",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == TINST_AMO(instruction)
    );

    TEST_SETUP_EXCEPT();
    value = amoand_d(vaddr_f,value);
    if(excpt.triggered) instruction = *((uint16_t*)excpt.epc) | *((uint16_t*)excpt.epc + 2) << 16;
    TEST_ASSERT("correct tinst when executing a amoand.d which results in a spf",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == TINST_AMO(instruction)
    );

    TEST_SETUP_EXCEPT();
    value = amoor_d(vaddr_f,value);
    if(excpt.triggered) instruction = *((uint16_t*)excpt.epc) | *((uint16_t*)excpt.epc + 2) << 16;
    TEST_ASSERT("correct tinst when executing a amoor.d which results in a spf",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == TINST_AMO(instruction)
    );

    TEST_SETUP_EXCEPT();
    value = amomin_d(vaddr_f,value);
    if(excpt.triggered) instruction = *((uint16_t*)excpt.epc) | *((uint16_t*)excpt.epc + 2) << 16;
    TEST_ASSERT("correct tinst when executing a amomin.d which results in a spf",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == TINST_AMO(instruction)
    );

    TEST_SETUP_EXCEPT();
    value = amomax_d(vaddr_f,value);
    if(excpt.triggered) instruction = *((uint16_t*)excpt.epc) | *((uint16_t*)excpt.epc + 2) << 16;
    TEST_ASSERT("correct tinst when executing a amomax.d which results in a spf",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == TINST_AMO(instruction)
    );

    TEST_SETUP_EXCEPT();
    value = amominu_d(vaddr_f,value);
    if(excpt.triggered) instruction = *((uint16_t*)excpt.epc) | *((uint16_t*)excpt.epc + 2) << 16;
    TEST_ASSERT("correct tinst when executing a amominu.d which results in a spf",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == TINST_AMO(instruction)
    );

    TEST_SETUP_EXCEPT();
    value = amomaxu_d(vaddr_f,value);
    if(excpt.triggered) instruction = *((uint16_t*)excpt.epc) | *((uint16_t*)excpt.epc + 2) << 16;
    TEST_ASSERT("correct tinst when executing a amomaxu.d which results in a spf",
        excpt.triggered == true && 
        excpt.cause == CAUSE_SPF &&
        excpt.tinst == TINST_AMO(instruction)
    );

    TEST_END();
}
