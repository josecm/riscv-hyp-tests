#include <rvh_test.h>

//enum test_state {};

extern test_func_t _test_table, _test_table_size;
test_func_t* test_table = &_test_table;
size_t test_table_size = (size_t) &_test_table_size;

unsigned test_num = 0;
unsigned curr_priv = PRIV_M;
unsigned real_priv = PRIV_M;

struct exception excpt;

uint64_t ecall_args[2];

uint64_t ecall(uint64_t a0, uint64_t a1)
{
    ecall_args[0] = a0;
    ecall_args[1] = a1;

    asm volatile("ecall" ::: "memory");

    return ecall_args[0];
}

static inline bool is_user(int priv) {
    return priv == PRIV_VU || priv == PRIV_HU;
}

void set_prev_priv(int priv){

    switch(curr_priv){
        case PRIV_M: {
            uint64_t temp = CSRR(mstatus);
            temp &= ~((3ULL << 11) | (1ULL << 39));
            if(priv == PRIV_VS || priv == PRIV_HS) temp |= (1ULL << 11);
            if(priv == PRIV_VU || priv == PRIV_VS) temp |= (1ULL << 39);
            CSRW(mstatus, temp);
        }
        break;
        case PRIV_HS:{
            uint64_t temp = CSRR(sstatus);
            temp &= ~(0x1ULL << 8);
            if(priv == PRIV_HS || priv == PRIV_VS) temp |= (0x1ULL << 8);
            CSRW(sstatus, temp);

            temp = CSRR(CSR_HSTATUS);
            temp &= ~(0x3ULL << 7);
            if(priv == PRIV_VS) temp |= (1ULL << 8);
            if(priv == PRIV_VU || priv == PRIV_VS) temp |= (0x1ULL << 7);
            CSRW(CSR_HSTATUS, temp);
        }
        break;
        case PRIV_VS: {
            uint64_t temp = CSRR(sstatus);
            CSRC(sstatus, 0x1ULL << 8);
        }
        break;
        default: ERROR("unknown current priv %d (%s)\n", priv, __func__);
    }
    
}

static inline void lower_priv(unsigned priv){

    if(priv == curr_priv) 
        return;

    if(priv >= curr_priv){
        ERROR("trying to \"lower\" to a higher priv");
    }

    if(curr_priv == PRIV_VS && priv == PRIV_HU){
        ERROR("trying to go from vs to hu");
    }

    set_prev_priv(priv);
    
    real_priv = priv;

    if(curr_priv == PRIV_M) {
        asm volatile(
            "la t0, 1f\n\t"
            "csrw mepc, t0\n\t"
            "mret\n\t"
            "1:\n\t"
            ::: "t0"
        );
    } else if(curr_priv == PRIV_VS || curr_priv == PRIV_HS){
        asm volatile(
            "la t0, 1f\n\t"
            "csrw sepc, t0\n\t"
            "sret\n\t"
            "1:\n\t"
            ::: "t0"
        );       
    } else {
        ERROR("cant lower priv from u");
    }

    curr_priv = priv;

}


void goto_priv(int target_priv){

    static bool on_going = false;

    DEBUG("goto_priv: real = %s, target = %s, curr = %s",  priv_strs[real_priv], priv_strs[target_priv], priv_strs[curr_priv]);

    if(is_user(target_priv) && is_user(curr_priv)) {
        goto_priv(PRIV_M);
    }

    if(real_priv == target_priv || target_priv >= PRIV_MAX){
        if(on_going)
            VERBOSE("...entered %s mode", priv_strs[target_priv]);
        on_going = false;
        curr_priv = target_priv;
        return;
    }

    if(!on_going)
        VERBOSE("entering %s mode...", priv_strs[target_priv]);

    on_going = true;

    if(target_priv > curr_priv){
        ecall(ECALL_GOTO_PRIV, target_priv);
    } else {
        VERBOSE("...entered %s mode", priv_strs[target_priv]);
        on_going = false;
        lower_priv(target_priv);
    }

}

static bool is_inst_fault(uint64_t cause){

    if(!(cause == CAUSE_IAM || cause == CAUSE_IAF || 
        cause == CAUSE_IPF || cause == CAUSE_IGPF)) {
            return false;
    }

    return true;
}

static bool is_ecall(uint64_t cause){

    if(!(cause == CAUSE_ECU || cause == CAUSE_ECS || 
        cause == CAUSE_ECV || cause == CAUSE_ECM)) {
            return false;
        }

    return true;
}

static inline uint64_t next_instruction(uint64_t epc){
    if(((*(uint16_t*)epc) & 0b11) == 0b11) return epc + 4;
    else return epc + 2;
}

#define return_from_exception(from_priv, to_priv, cause, epc) {\
    uint64_t _temp_status;\
    switch(from_priv){\
        case PRIV_M:\
            _temp_status = CSRR(mstatus);\
            _temp_status &= ~((3ULL << 11) | (1ULL << 39) | (1ULL << 7));\
            if(to_priv == PRIV_M) _temp_status |= (3ULL << 11);\
            if(to_priv == PRIV_VS || to_priv == PRIV_HS) _temp_status |= (1ULL << 11);\
            if(to_priv == PRIV_VU || to_priv == PRIV_VS) _temp_status |= (1ULL << 39);\
            CSRW(mstatus, _temp_status);\
            if(!(cause & (1ULL << 63))) CSRW(mepc, next_instruction(epc));\
        break;\
        case PRIV_HS:\
            _temp_status = CSRR(CSR_HSTATUS);\
            _temp_status &= ~(0x1ULL << 7);\
            if(to_priv == PRIV_VU || to_priv == PRIV_VS) _temp_status |= (0x1ULL << 7);\
            CSRW(CSR_HSTATUS, _temp_status);\
        case PRIV_VS:\
            _temp_status = CSRR(sstatus);\
            _temp_status &= ~((1ULL << 8) | (1ULL << 5));\
            if(to_priv == PRIV_VS || to_priv == PRIV_HS) _temp_status |= (0x1ULL << 8);\
            CSRW(sstatus, _temp_status);\
            if(!(cause & (1ULL << 63))) CSRW(sepc, next_instruction(epc));\
        break;\
    }\
    return from_priv;\
}

uint64_t mhandler(){

    real_priv = PRIV_M;

    uint64_t cause = CSRR(mcause);
    uint64_t epc = CSRR(mepc);
    uint64_t tval = CSRR(mtval);
    uint64_t tval2 = CSRR(CSR_MTVAL2);
    uint64_t tinst= CSRR(CSR_MTINST);

    VERBOSE("machine handler (mcause 0x%llx)", cause);
    DEBUG("mepc = 0x%lx", epc);
    DEBUG("mtval = 0x%lx", tval);
    DEBUG("mtval2 = 0x%lx", tval2);
    DEBUG("mtinst = 0x%lx", tinst);
    DEBUG("mpp = 0x%lx", (CSRR(mstatus) >> 11) & 0x3);
    DEBUG("mpv = 0x%lx", (CSRR(mstatus) >> 39) & 0x1);
    DEBUG("gva = 0x%lx", (CSRR(mstatus) >> MSTATUS_GVA_OFF) & 0x1);

    if(is_ecall(cause) && ecall_args[0] == ECALL_GOTO_PRIV){
        goto_priv(ecall_args[1]); 
    } else if(!excpt.testing){
        ERROR("untested exception!");
    }

    if(cause & (1ULL << 63)){
        CSRC(mip, 1ULL << (cause &  ~(1ULL << 63)));
    }

    excpt.triggered = true;
    excpt.priv = PRIV_M;
    excpt.cause = cause;
    excpt.epc = epc;
    excpt.tval = tval;
    excpt.tinst = tinst;
    excpt.tval2 = tval2;
    excpt.gva = !!((CSRR(mstatus) >> MSTATUS_GVA_OFF) & 0x1);
    excpt.xpv = !!((CSRR(mstatus) >> 39) & 0x1);
    excpt.testing = false;
    if(is_inst_fault(cause)){
        if(excpt.fault_inst != 0){ 
            epc = excpt.fault_inst;
            DEBUG("isntruction fault! return: 0x%lx", excpt.fault_inst);
        } else 
            ERROR("instruction fault without return address");
    }
    excpt.fault_inst = 0;

    unsigned temp_priv = real_priv;
    real_priv = curr_priv;
    return_from_exception(temp_priv, curr_priv, cause, epc);
}

uint64_t hshandler(){

    real_priv = PRIV_HS;

    uint64_t cause = CSRR(scause);
    uint64_t epc = CSRR(sepc);
    uint64_t tval = CSRR(stval);
    uint64_t htval = CSRR(CSR_HTVAL);
    uint64_t htinst = CSRR(CSR_HTINST);

    VERBOSE("hypervisor handler scause (scause = 0x%llx)", cause);
    DEBUG("sepc = 0x%lx", epc);
    DEBUG("stval = 0x%lx",  tval);
    DEBUG("htval = 0x%lx",  htval);
    DEBUG("htinst = 0x%lx",  htinst);
    DEBUG("hstatus = 0x%lx",  CSRR(CSR_HSTATUS));
    DEBUG("spv = 0x%lx", (CSRR(CSR_HSTATUS) >> 7) & 0x1);
    DEBUG("spvp = 0x%lx", (CSRR(CSR_HSTATUS) >> 8) & 0x1);
    DEBUG("gva = 0x%lx", (CSRR(CSR_HSTATUS) >> HSTATUS_GVA_OFF) & 0x1);

    if(is_ecall(cause) && ecall_args[0] == ECALL_GOTO_PRIV){
        goto_priv(ecall_args[1]); 
    } else if(is_ecall(cause)) {
        ERROR("unknown ecall"); 
    } else if(!excpt.testing){
        ERROR("untested exception!");
    }
    
    if(cause & (1ULL << 63)){
        CSRC(sip, 1ULL << (cause &  ~(1ULL << 63)));
        //CSRC(CSR_HVIP, 1ULL << (cause &  ~(1ULL << 63)));
        CSRC(CSR_HIP, 1ULL << (cause &  ~(1ULL << 63)));
    }

    excpt.triggered = true;
    excpt.priv = PRIV_HS;
    excpt.cause = cause;
    excpt.epc = epc;
    excpt.tval = tval;
    excpt.tinst = htinst;
    excpt.tval2 = htval;
    excpt.gva = !!((CSRR(CSR_HSTATUS) >> HSTATUS_GVA_OFF) & 0x1);
    excpt.xpv = !!((CSRR(CSR_HSTATUS) >> 7) & 0x1);
    excpt.testing = false;
    if(is_inst_fault(cause)){
        if(excpt.fault_inst != 0){ 
            epc = excpt.fault_inst;
            DEBUG("isntruction fault! return: 0x%lx", excpt.fault_inst);
        } else 
            ERROR("instruction fault without return address");
    }
    excpt.fault_inst = 0;

    unsigned temp_priv = real_priv;
    real_priv = curr_priv;
    return_from_exception(temp_priv, curr_priv, cause, epc);
}

uint64_t vshandler(){

    real_priv = PRIV_VS;

    uint64_t cause = CSRR(scause);
    uint64_t epc = CSRR(sepc);
    uint64_t tval = CSRR(stval);

    VERBOSE("virtual supervisor handler (scause = 0x%llx)", cause);
    DEBUG("scause = 0x%llx", cause);
    DEBUG("sepc = 0x%lx", epc);
    DEBUG("stval = 0x%lx", tval);
    
    if(is_ecall(cause) && ecall_args[0] == ECALL_GOTO_PRIV){
        goto_priv(ecall_args[1]); 
    } else if(!excpt.testing){
        ERROR("untested exception!");
    }
    
    if(cause & (1ULL << 63)){
        CSRC(sip, 1ULL << (cause &  ~(1ULL << 63)));
    }

    excpt.triggered = true;
    excpt.priv = PRIV_VS;
    excpt.cause = cause;
    excpt.epc = epc;
    excpt.tval = tval;
    excpt.testing = false;
    if(is_inst_fault(cause)){
        if(excpt.fault_inst != 0) epc = excpt.fault_inst;
        else ERROR("instruction fault without return address");
    }
    excpt.fault_inst = 0;

    unsigned temp_priv = real_priv;
    real_priv = curr_priv;
    return_from_exception(temp_priv, curr_priv, cause, epc);
}

extern void hshandler_entry();
extern void mhandler_entry();
extern void vshandler_entry();
    
void reset_state(){

    goto_priv(PRIV_M);
    CSRW(mstatus, 0ULL);
    //CSRW(mtvec, 0ULL);
    CSRW(medeleg, 0ULL);
    CSRW(mideleg, 0ULL);
    CSRW(mip, 0ULL);
    CSRW(mie, 0ULL);
    //CSRW(mtime, 0ULL); 
    //CSRW(mtimecmp, 0ULL); 
    CSRW(mscratch, 0ULL);
    CSRW(mepc, 0ULL);
    CSRW(mtval, 0ULL);
    CSRW(CSR_MTINST, 0ULL);
    CSRW(CSR_MTVAL2, 0ULL);
    //what about pmp register?
    CSRW(sstatus, 0ULL);
    //CSRW(stvec, 0ULL);
    CSRW(sip, 0ULL);
    CSRW(sie, 0ULL);
    CSRW(sscratch, 0ULL);
    CSRW(sepc, 0ULL);
    CSRW(scause, 0ULL);
    CSRW(stval, 0ULL);
    CSRW(satp, 0ULL);
    CSRW(CSR_HSTATUS, 0ULL);
    CSRW(CSR_HIDELEG, 0ULL);
    CSRW(CSR_HEDELEG, 0ULL);
    CSRW(CSR_HVIP, 0ULL);
    CSRW(CSR_HIP, 0ULL);
    CSRW(CSR_HIE, 0ULL);  
    CSRW(CSR_HTVAL, 0ULL);
    CSRW(CSR_HTINST, 0ULL);
    CSRW(CSR_HGATP, 0ULL);
    CSRW(CSR_VSSTATUS, 0ULL);
    CSRW(CSR_VSIP, 0ULL);
    CSRW(CSR_VSIE, 0ULL);
    //CSRW(CSR_VSTVEC, 0ULL);
    CSRW(CSR_VSSCRATCH, 0ULL);
    CSRW(CSR_VSEPC, 0ULL);
    CSRW(CSR_VSCAUSE, 0ULL);
    CSRW(CSR_VSTVAL, 0ULL);
    CSRW(CSR_VSATP, 0ULL);  

    CSRW(mtvec, mhandler_entry);
    CSRS(medeleg, (1ULL << 8) | (1ULL << 10));
    // full access to physical memory to other modes
    CSRW(pmpcfg0, 0xf);
    CSRW(pmpaddr0, (uint64_t) -1);

    CSRW(stvec, hshandler_entry);
    CSRS(CSR_HEDELEG, (1ULL << 8));

    CSRW(CSR_VSTVEC, vshandler_entry);

    sfence();
    hfence();
}
