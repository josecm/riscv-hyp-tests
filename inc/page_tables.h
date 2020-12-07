#ifndef PAGE_TABLES_H
#define PAGE_TABLES_H

#include <rvh_test.h>

#define PAGE_SIZE 0x1000ULL
#define PT_SIZE (PAGE_SIZE)
#define PAGE_ADDR_MSK (~(PAGE_SIZE - 1))
#define PAGE_SHIFT (12)
#define SUPERPAGE_SIZE(N) ((PAGE_SIZE) << (((2-N))*9))

#define PT_LVLS (3)  // assumes sv39 for rv64
#define PTE_INDEX_SHIFT(LEVEL) ((9 * (PT_LVLS - 1 - (LEVEL))) + 12)
#define PTE_ADDR_MSK BIT_MASK(12, 44)

#define PTE_INDEX(LEVEL, ADDR) (((ADDR) >> PTE_INDEX_SHIFT(LEVEL)) & (0x1FF))
#define PTE_FLAGS_MSK BIT_MASK(0, 8)

#define PTE_VALID (1ULL << 0)
#define PTE_READ (1ULL << 1)
#define PTE_WRITE (1ULL << 2)
#define PTE_EXECUTE (1ULL << 3)
#define PTE_USER (1ULL << 4)
#define PTE_GLOBAL (1ULL << 5)
#define PTE_ACCESS (1ULL << 6)
#define PTE_DIRTY (1ULL << 7)

#define PTE_V PTE_VALID
#define PTE_AD (PTE_ACCESS | PTE_DIRTY)
#define PTE_U PTE_USER
#define PTE_R (PTE_READ)
#define PTE_RW (PTE_READ | PTE_WRITE)
#define PTE_X (PTE_EXECUTE)
#define PTE_RX (PTE_READ | PTE_EXECUTE)
#define PTE_RWX (PTE_READ | PTE_WRITE | PTE_EXECUTE)

#define PTE_RSW_OFF 8
#define PTE_RSW_LEN 2
#define PTE_RSW_MSK BIT_MASK(PTE_RSW_OFF, PTE_RSW_LEN)

#define PTE_TABLE (0)
#define PTE_PAGE (PTE_RWX)
#define PTE_SUPERPAGE (PTE_PAGE)

/* ------------------------------------------------------------- */

#define TEST_PPAGE_BASE (MEM_BASE+(MEM_SIZE/2))
#define TEST_VPAGE_BASE (0x100000000)

enum test_page { 
    VSRWX_GURWX,
    VSRWX_GURW,
    VSRWX_GURX,
    VSRWX_GUR,
    VSRWX_GUX,
    VSRW_GURWX,
    VSRW_GURW,
    VSRW_GURX,
    VSRW_GUR,
    VSRW_GUX,
    VSRX_GURWX,
    VSRX_GURW,
    VSRX_GURX,
    VSRX_GUR,
    VSRX_GUX,
    VSR_GURWX,
    VSR_GURW,
    VSR_GURX,
    VSR_GUR,
    VSR_GUX,
    VSX_GURWX,
    VSX_GURW,
    VSX_GURX,
    VSX_GUR,
    VSX_GUX,
    VSURWX_GRWX,
    VSURWX_GRW,
    VSURWX_GRX,
    VSURWX_GR,
    VSURWX_GX,
    VSURW_GRWX,
    VSURW_GRW,
    VSURW_GRX,
    VSURW_GR,
    VSURW_GX,
    VSURX_GRWX,
    VSURX_GRW,
    VSURX_GRX,
    VSURX_GR,
    VSURX_GX,
    VSUR_GRWX,
    VSUR_GRW,
    VSUR_GRX,
    VSUR_GR,
    VSUR_GX,
    VSUX_GRWX,
    VSUX_GRW,
    VSUX_GRX,
    VSUX_GR,
    VSUX_GX,
    VSURWX_GURWX,
    VSURWX_GURW,
    VSURWX_GURX,
    VSURWX_GUR,
    VSURWX_GUX,
    VSURW_GURWX,
    VSURW_GURW,
    VSURW_GURX,
    VSURW_GUR,
    VSURW_GUX,
    VSURX_GURWX,
    VSURX_GURW,
    VSURX_GURX,
    VSURX_GUR,
    VSURX_GUX,
    VSUR_GURWX,
    VSUR_GURW,
    VSUR_GURX,
    VSUR_GUR,
    VSUR_GUX,
    VSUX_GURWX,
    VSUX_GURW,
    VSUX_GURX,
    VSUX_GUR,
    VSUX_GUX,
    VSRWX_GRWX,
    VSRWX_GRW,
    VSRWX_GRX,
    VSRWX_GR,
    VSRWX_GX,
    VSRW_GRWX,
    VSRW_GRW,
    VSRW_GRX,
    VSRW_GR,
    VSRW_GX,
    VSRX_GRWX,
    VSRX_GRW,
    VSRX_GRX,
    VSRX_GR,
    VSRX_GX,
    VSR_GRWX,
    VSR_GRW,
    VSR_GRX,
    VSR_GR,
    VSR_GX,
    VSX_GRWX,
    VSX_GRW,
    VSX_GRX,
    VSX_GR,
    VSX_GX,
    VSI_GI,
    VSRWX_GI,
    VSRW_GI,
    VSI_GURWX,
    VSI_GUX,
    VSI_GUR,
    VSI_GURW,
    SCRATCHPAD,
    SWITCH1,
    SWITCH2,
    TOP = 511,
    TEST_PAGE_MAX
};

typedef uint64_t pte_t;

extern pte_t hspt[][512];
extern pte_t vspt[][512];
extern pte_t hpt_root[];
extern pte_t hpt[][512];

static inline uintptr_t vs_page_base(enum test_page tp){
    if(tp < TEST_PAGE_MAX){
        return (uintptr_t)(TEST_VPAGE_BASE+(tp*PAGE_SIZE));
    } else {
        ERROR("trying to get invalid test page address");
    }
}

static inline uintptr_t hs_page_base(enum test_page tp){
    return vs_page_base(tp);
}

static inline uintptr_t vs_page_base_limit(enum test_page tp){
    if(tp < TEST_PAGE_MAX){
        return (uintptr_t)((0x20000000000-0x200000)+(tp*PAGE_SIZE));
    } else {
        ERROR("trying to get invalid test page address");
    }
}

static inline uintptr_t phys_page_base(enum test_page tp){
    if(tp < TEST_PAGE_MAX){
        return (uintptr_t)(TEST_PPAGE_BASE+(tp*PAGE_SIZE));
    } else {
        ERROR("trying to get invalid test page address");
    }
}

void hspt_init();
void vspt_init();
void hpt_init();
void hspt_switch();
void vspt_switch();
void hpt_switch();

#endif /* PAGE_TABLES_H */
