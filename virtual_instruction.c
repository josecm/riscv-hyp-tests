#include <rvh_test.h>
#include <page_tables.h> 


bool virtual_instruction() {

    TEST_START();

    goto_priv(PRIV_VS);
    TEST_SETUP_EXCEPT();
    hfence_vvma();
    TEST_ASSERT("vs executing hfence.vvma leads to virtual isntruction exception",
        excpt.triggered == true &&
        excpt.cause == CAUSE_VRTI
    ); 
    
    goto_priv(PRIV_VS);
    TEST_SETUP_EXCEPT();
    hfence_gvma();
    TEST_ASSERT("vs executing hfence.gvma leads to virtual isntruction exception",
        excpt.triggered == true &&
        excpt.cause == CAUSE_VRTI
    ); 

    ////////////////////////////////////////////////////////////////////////

    goto_priv(PRIV_VS);
    TEST_SETUP_EXCEPT();
    volatile uint64_t tmp = hlvd(0);
    TEST_ASSERT("vs hlvd leads to virtual isntruction exception",
        excpt.triggered == true &&
        excpt.cause == CAUSE_VRTI
    ); 

    ////////////////////////////////////////////////////////////////////////

    goto_priv(PRIV_M);
    CSRS(CSR_HSTATUS, HSTATUS_VTSR);
    goto_priv(PRIV_VS);
    TEST_SETUP_EXCEPT();
    TEST_EXEC_SRET();
    TEST_ASSERT("vs sret leads to virtual instruction exception when vtsr set",
        excpt.triggered == true &&
        excpt.cause == CAUSE_VRTI
    );  
    goto_priv(PRIV_M);
    CSRC(CSR_HSTATUS, HSTATUS_VTSR);

    ////////////////////////////////////////////////////////////////////////

    goto_priv(PRIV_M);
    CSRS(CSR_HSTATUS, HSTATUS_VTVM);
    goto_priv(PRIV_VS);
    TEST_SETUP_EXCEPT();
    sfence();
    TEST_ASSERT("vs sfence leads to virtual instruction exception when vtvm set",
        excpt.triggered == true &&
        excpt.cause == CAUSE_VRTI
    );  

    TEST_SETUP_EXCEPT();
    CSRW(satp, 0x0);
    TEST_ASSERT("vs satp acess leads to virtual instruction exception when vtvm set",
        excpt.triggered == true &&
        excpt.cause == CAUSE_VRTI
    );  
    goto_priv(PRIV_M);
    CSRC(CSR_HSTATUS, HSTATUS_VTVM);

    ////////////////////////////////////////////////////////////////////////

    goto_priv(PRIV_M);
    CSRS(CSR_HSTATUS, HSTATUS_VTW);
    goto_priv(PRIV_VS);
    TEST_SETUP_EXCEPT();
    asm("wfi");
    TEST_ASSERT("vs wfi leads to virtual instruction exception when vtw set",
        excpt.triggered == true &&
        excpt.cause == CAUSE_VRTI
    );  

    /**
     * We can test this the other way around because it will stall the program
     */

    ////////////////////////////////////////////////////////////////////////

    goto_priv(PRIV_M);
    CSRW(mcounteren, 0);
    CSRW(CSR_HCOUNTEREN, 0);

    goto_priv(PRIV_VS);
    TEST_SETUP_EXCEPT();
    volatile uint64_t time = CSRR(time);
    TEST_ASSERT("vs access to time casuses virtual instruction exception",
        excpt.triggered == true &&
        excpt.cause == CAUSE_ILI
    );

    goto_priv(PRIV_M);
    CSRS(mcounteren, HCOUNTEREN_TM);
    CSRS(CSR_HCOUNTEREN, HCOUNTEREN_TM);
    goto_priv(PRIV_VS);
    TEST_SETUP_EXCEPT();
    time = CSRR(time);
    TEST_ASSERT("vs access to time casuses succsseful with mcounteren.tm and hcounteren.tm set",
        excpt.triggered == true &&
        excpt.cause == CAUSE_ILI
    );

    //////////////////////////////////////////////////////////////////////

    goto_priv(PRIV_M);
    CSRW(mcounteren, 0);
    CSRW(CSR_HCOUNTEREN, 0);
    goto_priv(PRIV_VS);
    TEST_SETUP_EXCEPT();
    CSRW(sscratch, 1);
    volatile uint64_t cycle = CSRR(cycle);
    TEST_ASSERT("vs access to cycle casuses virtual instruction exception",
        excpt.triggered == true &&
        excpt.cause == CAUSE_ILI
    );

    goto_priv(PRIV_M);
    CSRS(mcounteren, HCOUNTEREN_CY);
    CSRW(CSR_HCOUNTEREN, 0);
    goto_priv(PRIV_VS);
    TEST_SETUP_EXCEPT();
    CSRW(sscratch, 1);
    cycle = CSRR(cycle);
    TEST_ASSERT("vs access to cycle casuses virtual instruction exception when mcounteren.cy set",
        excpt.triggered == true &&
        excpt.cause == CAUSE_VRTI
    );


    goto_priv(PRIV_M);
    CSRS(mcounteren, HCOUNTEREN_CY);
    CSRS(CSR_HCOUNTEREN, HCOUNTEREN_CY);
    goto_priv(PRIV_VS);
    TEST_SETUP_EXCEPT();
    CSRW(sscratch, 1);
    cycle = CSRR(cycle);
    TEST_ASSERT("vs access to cycle casuses succsseful when mcounteren.cy and hcounteren.cy set",
        excpt.triggered == false
    );

    TEST_END(); 
}
