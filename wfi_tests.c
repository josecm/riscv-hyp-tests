#include <rvh_test.h>
#include <csrs.h> 

bool wfi_exception_tests() {

    TEST_START();

    TEST_SETUP_EXCEPT();

    goto_priv(PRIV_HU);
    wfi();
    TEST_ASSERT("U-mode wfi causes illegal instruction exception",
        excpt.triggered == true &&  
        excpt.cause == CAUSE_ILI
    );   

    goto_priv(PRIV_VU);
    TEST_SETUP_EXCEPT();
    wfi();
    TEST_ASSERT("VU-mode wfi causes illegal instruction exception",
        excpt.triggered == true &&  
        excpt.cause == CAUSE_VRTI
    ); 

    /* Keep an interrupt pending so we don't get stuck when wfi succeeds */
    goto_priv(PRIV_M);
    CSRS(mie, 0b0100);
    CSRS(mip, 0b0100);

    goto_priv(PRIV_M);
    TEST_SETUP_EXCEPT();
    wfi();
    TEST_ASSERT("machine mode wfi does not trigger exception",
        excpt.triggered == false
    );  

    /** 
     * Delegate the pending interrupt to hs mode so it doesnt immediately 
     * trigger a trap to machine when we jump to hs.
     */
    goto_priv(PRIV_M);
    CSRS(mideleg, 0b0100); 
    CSRS(CSR_HIE, 0b0100);

    goto_priv(PRIV_HS);
    TEST_SETUP_EXCEPT();
    wfi();
    TEST_ASSERT("S-mode wfi does not trigger exception",
        excpt.triggered == false
    );   

    goto_priv(PRIV_M);
    CSRS(mstatus, MSTATUS_TW);

    goto_priv(PRIV_HS);
    TEST_SETUP_EXCEPT();
    wfi();
    TEST_ASSERT("S-mode wfi triggers illegal instructions exception when mstatus.tw = 1",
        excpt.triggered == true &&  
        excpt.cause == CAUSE_ILI
    );   

    /** 
     * Delegate the pending interrupt to hs mode so it doesnt immediately 
     * trigger a trap to machine when we jump to vs.
     */
    CSRS(CSR_HIDELEG, 0b0100); 
    CSRS(CSR_VSIE, 0b0010); 

    goto_priv(PRIV_VS);
    TEST_SETUP_EXCEPT();
    wfi();
    TEST_ASSERT("VS-mode wfi causes illegal instruction exception when mstatus.tw = 1",
        excpt.triggered == true &&  
        excpt.cause == CAUSE_ILI
    );   

    goto_priv(PRIV_M);
    CSRC(mstatus, MSTATUS_TW);

    goto_priv(PRIV_VS);
    TEST_SETUP_EXCEPT();
    wfi();
    TEST_ASSERT("VS-mode wfi does not trap when mstatus.tw = 0 and hstatus.vtw = 0",
        excpt.triggered == false
    );   

    goto_priv(PRIV_M);
    CSRS(CSR_HSTATUS, HSTATUS_VTW);  

    goto_priv(PRIV_VS);
    TEST_SETUP_EXCEPT();
    wfi();
    TEST_ASSERT("VS-mode wfi triggers virtual inst. exception  when hstatus.vtw = 1",
        excpt.triggered == true &&
        excpt.cause == CAUSE_VRTI
    ); 

    TEST_END();
}