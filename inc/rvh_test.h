#ifndef RVH_TESTS
#define RVH_TESTS

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <csrs.h>
#include <instructions.h>
#include <platform.h>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#ifndef LOG_LEVEL
#define LOG_LEVEL   LOG_INFO
#endif

#define CDFLT  "\x1B[0m"
#define CRED  "\x1B[31m"
#define CGRN  "\x1B[32m"
#define CYEL  "\x1B[33m"
#define CBLU  "\x1B[34m"
#define CMAG  "\x1B[35m"
#define CCYN  "\x1B[36m"
#define CWHT  "\x1B[37m"

#define LOG_NONE      (0)	
#define LOG_ERROR	  (1)
#define LOG_INFO      (2)
#define LOG_DETAIL    (3)
#define LOG_WARNING   (4)
#define LOG_VERBOSE   (5)
#define LOG_DEBUG     (6)

#if LOG_LEVEL >= LOG_ERROR
# define ERROR(str...)	{\
    printf(CRED "ERROR: " CDFLT str );\
    printf(" (%s, %d)\n", __func__, __LINE__);\
    exit(0);\
    while(1);\
}
#else
# define ERROR(...) { exit(-1); while(1); }
#endif

#if LOG_LEVEL >= LOG_INFO
# define INFO(str...)	{ printf(str); printf("\n"); }
#else
# define INFO(...)
#endif

#if LOG_LEVEL >= LOG_DETAIL
# define DETAIL(str...)	{ printf(str); printf("\n"); }
#else
# define DETAIL(...)
#endif

#if LOG_LEVEL >= LOG_WARNING
# define WARN(str...)	{ printf(CYEL "WARNING: " CDFLT str); printf("\n"); }
#else
# define WARN(...)
#endif

#if LOG_LEVEL >= LOG_VERBOSE
# define VERBOSE(str...)	{ printf("VERBOSE: " str); printf("\n"); }
#else
# define VERBOSE(...)
#endif

#if LOG_LEVEL >= LOG_DEBUG
# define DEBUG(str...)	{ printf("DEBUG: " str); printf("\n"); }
#else
# define DEBUG(...)
#endif

enum priv {PRIV_VU = 0, PRIV_HU = 1, PRIV_VS = 2, PRIV_HS = 3, PRIV_M = 4, PRIV_MAX};
extern unsigned curr_priv;

static const char* priv_strs[] = {
    [PRIV_VU] = "vu",
    [PRIV_VS] = "vs",
    [PRIV_HU] = "hu",
    [PRIV_HS] = "hs",
    [PRIV_M] = "m",
};

enum ecall {ECALL_GOTO_PRIV = 1};

extern struct exception {
    bool testing;
    bool triggered;
    enum priv priv;
    uint64_t cause;
    uint64_t epc;
    uint64_t tval;
    uint64_t tinst;
    uint64_t tval2;
    bool gva;
    bool xpv;
    uintptr_t fault_inst;
} excpt;

typedef bool (*test_func_t)();
extern test_func_t* test_table;
extern size_t test_table_size;

#define TEST_START()\
    const char* __test_name = __func__;\
    bool test_status = true;\
    if(LOG_LEVEL >= LOG_INFO) printf(CBLU "%-70s" CDFLT, __test_name);\
    if(LOG_LEVEL >= LOG_DETAIL) printf("\n");

#define TEST_REGISTER(test)\
    bool test();\
    static test_func_t test ## func __attribute__((section(".test_table"), used)) = test;

#define TEST_ASSERT(test, cond, ...) {\
    if(LOG_LEVEL >= LOG_DETAIL){\
        size_t line_size = 60;\
        size_t size = strlen(test);\
        printf(CBLU "\t%-70.*s" CDFLT, line_size, test);\
        for(int i = line_size; i < size; i+=line_size)\
            printf(CBLU "\n\t%-70.*s" CDFLT, line_size, &test[i]);\
        printf("%s" CDFLT, (cond) ? CGRN "PASSED" : CRED "FAILED");\
        if(!(cond)) { printf("\n\t("); printf(""__VA_ARGS__); printf(")"); }\
        printf("\n");\
    }\
    test_status = test_status && cond;\
    /*if(!test_status) goto failed; /**/\
}

#define TEST_SETUP_EXCEPT() {\
    excpt.testing = true;\
    excpt.triggered = false;\
    excpt.fault_inst = 0;\
    DEBUG("setting up exception test");\
}


#define TEST_EXEC_EXCEPT(addr) {\
    asm volatile(\
        "la t0, 1f\n\t"\
        "sd t0, %1\n\t"\
        "1:\n\t"\
        "jr  %0\n\t"\
        :: "r"(addr), "m"(excpt.fault_inst) : "t0", "memory"\
    );\
}

#define TEST_EXEC_SRET() {\
    asm volatile(\
        "la t0, 1f\n\t"\
        "csrw sepc, t0\n\t"\
        "sret\n\t"\
        "1:\n\t"\
        ::: "t0", "memory"\
    );\
}


#define TEST_END(test) {\
failed:\
    if(LOG_LEVEL >= LOG_INFO && LOG_LEVEL < LOG_VERBOSE){\
         printf("%s\n" CDFLT, (test_status) ? CGRN "PASSED" : CRED "FAILED");\
    }\
    goto_priv(PRIV_M);\
    reset_state();\
    return (test_status);\
}

#define INFO_PRINT(var) {\
    INFO(#var ": 0x%llx", (var));\
}

#define INFO_PRINT_CSR(csr){\
    INFO(#csr ": 0x%llx", CSRR(csr));\
}

#define INFO_PRINT_XCEPT(){\
    INFO("excpt.triggered = %d", excpt.triggered);\
    INFO("excpt.priv = %s", priv_strs[excpt.priv]);\
    INFO("excpt.cause = 0x%llx", excpt.cause);\
    INFO("excpt.tval = 0x%llx", excpt.tval);\
    INFO("excpt.tval2 = 0x%llx", excpt.tval2);\
    INFO("excpt.tinst = 0x%llx", excpt.tinst);\
}

#define DEBUG_PRINT(var) {\
    DEBUG(#var ": 0x%llx", (var));\
}

#define DEBUG_PRINT_CSR(csr){\
    DEBUG(#csr ": 0x%llx", CSRR(csr));\
}

#define DEBUG_PRINT_XCEPT(){\
    DEBUG("excpt.triggered = %d", excpt.triggered);\
    DEBUG("excpt.priv = %s", priv_strs[excpt.priv]);\
    DEBUG("excpt.cause = 0x%llx", excpt.cause);\
    DEBUG("excpt.tval = 0x%llx", excpt.tval);\
    DEBUG("excpt.tval2 = 0x%llx", excpt.tval2);\
    INFO("excpt.tinst = 0x%llx", excpt.tinst);\
}

#define check_csr_wrrd(name, addr, wr, rd){\
    uint64_t temp = CSRR(addr);\
    CSRW(addr, wr);\
    uint64_t val = CSRR(addr);\
    TEST_ASSERT(name, (rd) == (val), "%016lx %016lx %016lx", wr, val, rd);\
    CSRW(addr, temp);\
}

#define check_csr_rd(name, addr, rd){\
    uint64_t val = CSRR(addr);\
    TEST_ASSERT(name, ((rd) == (val)), "%16c %016lx %016lx", '-', val, rd);\
}

static inline uint64_t read64(uintptr_t addr){
    return *((volatile uint64_t*) addr);
}

static inline uint32_t read32(uintptr_t addr){
    return *((volatile uint32_t*) addr);
}

static inline uint16_t read16(uintptr_t addr){
    return *((volatile uint16_t*) addr);    
}

static inline uint8_t read8(uintptr_t addr){
    return *((volatile uint8_t*) addr);    
}

static inline void write64(uintptr_t addr, uint64_t val){
    *((volatile uint64_t*) addr) = val;
}

static inline void write32(uintptr_t addr, uint32_t val){
    *((volatile uint32_t*) addr) = val;
}

static inline void write16(uintptr_t addr, uint16_t val){
    *((volatile uint16_t*) addr) = val;    
}

static inline void write8(uintptr_t addr, uint8_t val){
    *((volatile uint8_t*) addr) = val;    
}

static inline uint32_t read_instruction(uintptr_t addr) {

    uint32_t instruction = 0;

    if((addr & 0b1) != 0) {
        ERROR("trying to read unaligned instruction (%s, %d)", __func__, __LINE__);
    }

    instruction = *((uint16_t*)addr);
    if(!INS_COMPRESSED(instruction)) {
        instruction |= (*((uint16_t*)(addr + 2)) << 16);
    }

    return instruction;
}

void reset_state();
void set_prev_priv(int target_priv);
void goto_priv(int target_priv);

#endif
