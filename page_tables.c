#include <stdint.h>
#include <stdio.h>
#include <page_tables.h> 
#include <csrs.h>


#if ((MEM_BASE & (SUPERPAGE_SIZE(0)-1)) != 0)
#   error "MEM_BASE must be aligned to level 0 superpage size"
#endif

#if (MEM_BASE >= (SUPERPAGE_SIZE(0)*4))
#   error "MEM_BASE less than four time level 0 superpage size"
#endif

#if (MEM_SIZE < (0x10000000))
#   error "MEM_SIZE not enough (less than 256MB)"
#endif

struct {
    uint64_t vs;
    uint64_t h;
} test_page_perm_table[TEST_PAGE_MAX] = {
    [VSRWX_GRWX]      =   {PTE_V | PTE_RWX,         PTE_V | PTE_RWX},     
    [VSRWX_GRW]       =   {PTE_V | PTE_RWX,         PTE_V | PTE_RW},     
    [VSRWX_GRX]       =   {PTE_V | PTE_RWX,         PTE_V | PTE_RX},     
    [VSRWX_GR]        =   {PTE_V | PTE_RWX,         PTE_V | PTE_R},
    [VSRWX_GX]        =   {PTE_V | PTE_RWX,         PTE_V | PTE_X},     
    [VSRW_GRWX]       =   {PTE_V | PTE_RW,          PTE_V | PTE_RWX},     
    [VSRW_GRW]        =   {PTE_V | PTE_RW,          PTE_V | PTE_RW},      
    [VSRW_GRX]        =   {PTE_V | PTE_RW,          PTE_V | PTE_RX},  
    [VSRW_GR]         =   {PTE_V | PTE_RW,          PTE_V | PTE_R},    
    [VSRW_GX]         =   {PTE_V | PTE_RW,          PTE_V | PTE_X},       
    [VSRX_GRWX]       =   {PTE_V | PTE_RX,          PTE_V | PTE_RWX},    
    [VSRX_GRW]        =   {PTE_V | PTE_RX,          PTE_V | PTE_RW},     
    [VSRX_GRX]        =   {PTE_V | PTE_RX,          PTE_V | PTE_RX},
    [VSRX_GR]         =   {PTE_V | PTE_RX,          PTE_V | PTE_R},    
    [VSRX_GX]         =   {PTE_V | PTE_RX,          PTE_V | PTE_X}, 
    [VSR_GRWX]        =   {PTE_V | PTE_R,           PTE_V | PTE_RWX},   
    [VSR_GRW]         =   {PTE_V | PTE_R,           PTE_V | PTE_RW},    
    [VSR_GRX]         =   {PTE_V | PTE_R,           PTE_V | PTE_RX},
    [VSR_GR]          =   {PTE_V | PTE_R,           PTE_V | PTE_R},    
    [VSR_GX]          =   {PTE_V | PTE_R,           PTE_V | PTE_X},      
    [VSX_GRWX]        =   {PTE_V | PTE_X,           PTE_V | PTE_RWX},   
    [VSX_GRW]         =   {PTE_V | PTE_X,           PTE_V | PTE_RW},    
    [VSX_GRX]         =   {PTE_V | PTE_X,           PTE_V | PTE_RX}, 
    [VSX_GR]          =   {PTE_V | PTE_X,           PTE_V | PTE_R},   
    [VSX_GX]          =   {PTE_V | PTE_X,           PTE_V | PTE_X},  
    [VSRWX_GURWX]     =   {PTE_V | PTE_RWX,         PTE_V | PTE_U | PTE_RWX},            
    [VSRWX_GURW]      =   {PTE_V | PTE_RWX,         PTE_V | PTE_U | PTE_RW},             
    [VSRWX_GURX]      =   {PTE_V | PTE_RWX,         PTE_V | PTE_U | PTE_RX}, 
    [VSRWX_GUR]       =   {PTE_V | PTE_RWX,         PTE_V | PTE_U | PTE_R},            
    [VSRWX_GUX]       =   {PTE_V | PTE_RWX,         PTE_V | PTE_U | PTE_X},          
    [VSRW_GURWX]      =   {PTE_V | PTE_RW,          PTE_V | PTE_U | PTE_RWX},            
    [VSRW_GURW]       =   {PTE_V | PTE_RW,          PTE_V | PTE_U | PTE_RW},         
    [VSRW_GURX]       =   {PTE_V | PTE_RW,          PTE_V | PTE_U | PTE_RX},
    [VSRW_GUR]        =   {PTE_V | PTE_RW,          PTE_V | PTE_U | PTE_R},         
    [VSRW_GUX]        =   {PTE_V | PTE_RW,          PTE_V | PTE_U | PTE_X},          
    [VSRX_GURWX]      =   {PTE_V | PTE_RX,          PTE_V | PTE_U | PTE_RWX},            
    [VSRX_GURW]       =   {PTE_V | PTE_RX,          PTE_V | PTE_U | PTE_RW},         
    [VSRX_GURX]       =   {PTE_V | PTE_RX,          PTE_V | PTE_U | PTE_RX}, 
    [VSRX_GUR]        =   {PTE_V | PTE_RX,          PTE_V | PTE_U | PTE_R},        
    [VSRX_GUX]        =   {PTE_V | PTE_RX,          PTE_V | PTE_U | PTE_X},    
    [VSR_GURWX]       =   {PTE_V | PTE_R,           PTE_V | PTE_U | PTE_RWX},        
    [VSR_GURW]        =   {PTE_V | PTE_R,           PTE_V | PTE_U | PTE_RW},         
    [VSR_GURX]        =   {PTE_V | PTE_R,           PTE_V | PTE_U | PTE_RX},
    [VSR_GUR]         =   {PTE_V | PTE_R,           PTE_V | PTE_U | PTE_R},         
    [VSR_GUX]         =   {PTE_V | PTE_R,           PTE_V | PTE_U | PTE_X},      
    [VSX_GURWX]       =   {PTE_V | PTE_X,           PTE_V | PTE_U | PTE_RWX},        
    [VSX_GURW]        =   {PTE_V | PTE_X,           PTE_V | PTE_U | PTE_RW},         
    [VSX_GURX]        =   {PTE_V | PTE_X,           PTE_V | PTE_U | PTE_RX},   
    [VSX_GUR]         =   {PTE_V | PTE_X,           PTE_V | PTE_U | PTE_R},      
    [VSX_GUX]         =   {PTE_V | PTE_X,           PTE_V | PTE_U | PTE_X},          
    [VSURWX_GRWX]     =   {PTE_V | PTE_U | PTE_RWX, PTE_V | PTE_RWX},            
    [VSURWX_GRW]      =   {PTE_V | PTE_U | PTE_RWX, PTE_V | PTE_RW},         
    [VSURWX_GRX]      =   {PTE_V | PTE_U | PTE_RWX, PTE_V | PTE_RX},
    [VSURWX_GR]       =   {PTE_V | PTE_U | PTE_RWX, PTE_V | PTE_R},         
    [VSURWX_GX]       =   {PTE_V | PTE_U | PTE_RWX, PTE_V | PTE_X},          
    [VSURW_GRWX]      =   {PTE_V | PTE_U | PTE_RW,  PTE_V | PTE_RWX},        
    [VSURW_GRW]       =   {PTE_V | PTE_U | PTE_RW,  PTE_V | PTE_RW},         
    [VSURW_GRX]       =   {PTE_V | PTE_U | PTE_RW,  PTE_V | PTE_RX},
    [VSURW_GR]        =   {PTE_V | PTE_U | PTE_RW,  PTE_V | PTE_R},         
    [VSURW_GX]        =   {PTE_V | PTE_U | PTE_RW,  PTE_V | PTE_X},          
    [VSURX_GRWX]      =   {PTE_V | PTE_U | PTE_RX,  PTE_V | PTE_RWX},        
    [VSURX_GRW]       =   {PTE_V | PTE_U | PTE_RX,  PTE_V | PTE_RW},         
    [VSURX_GRX]       =   {PTE_V | PTE_U | PTE_RX,  PTE_V | PTE_RX},
    [VSURX_GR]        =   {PTE_V | PTE_U | PTE_RX,  PTE_V | PTE_R},         
    [VSURX_GX]        =   {PTE_V | PTE_U | PTE_RX,  PTE_V | PTE_X},    
    [VSUR_GRWX]       =   {PTE_V | PTE_U | PTE_R,   PTE_V | PTE_RWX},        
    [VSUR_GRW]        =   {PTE_V | PTE_U | PTE_R,   PTE_V | PTE_RW},         
    [VSUR_GRX]        =   {PTE_V | PTE_U | PTE_R,   PTE_V | PTE_RX}, 
    [VSUR_GR]         =   {PTE_V | PTE_U | PTE_R,   PTE_V | PTE_R},        
    [VSUR_GX]         =   {PTE_V | PTE_U | PTE_R,   PTE_V | PTE_X},         
    [VSUX_GRWX]       =   {PTE_V | PTE_U | PTE_X,   PTE_V | PTE_RWX},        
    [VSUX_GRW]        =   {PTE_V | PTE_U | PTE_X,   PTE_V | PTE_RW},         
    [VSUX_GRX]        =   {PTE_V | PTE_U | PTE_X,   PTE_V | PTE_RX}, 
    [VSUX_GR]         =   {PTE_V | PTE_U | PTE_X,   PTE_V | PTE_R},        
    [VSUX_GX]         =   {PTE_V | PTE_U | PTE_X,   PTE_V | PTE_X},          
    [VSURWX_GURWX]    =   {PTE_V | PTE_U | PTE_RWX, PTE_V | PTE_U | PTE_RWX},            
    [VSURWX_GURW]     =   {PTE_V | PTE_U | PTE_RWX, PTE_V | PTE_U | PTE_RW},             
    [VSURWX_GURX]     =   {PTE_V | PTE_U | PTE_RWX, PTE_V | PTE_U | PTE_RX},
    [VSURWX_GUR]      =   {PTE_V | PTE_U | PTE_RWX, PTE_V | PTE_U | PTE_R},              
    [VSURWX_GUX]      =   {PTE_V | PTE_U | PTE_RWX, PTE_V | PTE_U | PTE_X},          
    [VSURW_GURWX]     =   {PTE_V | PTE_U | PTE_RW,  PTE_V | PTE_U | PTE_RWX},            
    [VSURW_GURW]      =   {PTE_V | PTE_U | PTE_RW,  PTE_V | PTE_U | PTE_RW},         
    [VSURW_GURX]      =   {PTE_V | PTE_U | PTE_RW,  PTE_V | PTE_U | PTE_RX}, 
    [VSURW_GUR]       =   {PTE_V | PTE_U | PTE_RW,  PTE_V | PTE_U | PTE_R},        
    [VSURW_GUX]       =   {PTE_V | PTE_U | PTE_RW,  PTE_V | PTE_U | PTE_X},          
    [VSURX_GURWX]     =   {PTE_V | PTE_U | PTE_RX,  PTE_V | PTE_U | PTE_RWX},            
    [VSURX_GURW]      =   {PTE_V | PTE_U | PTE_RX,  PTE_V | PTE_U | PTE_RW},         
    [VSURX_GURX]      =   {PTE_V | PTE_U | PTE_RX,  PTE_V | PTE_U | PTE_RX},
    [VSURX_GUR]       =   {PTE_V | PTE_U | PTE_RX,  PTE_V | PTE_U | PTE_R},         
    [VSURX_GUX]       =   {PTE_V | PTE_U | PTE_RX,  PTE_V | PTE_U | PTE_X},   
    [VSUR_GURWX]      =   {PTE_V | PTE_U | PTE_R,   PTE_V | PTE_U | PTE_RWX},
    [VSUR_GURW]       =   {PTE_V | PTE_U | PTE_R,   PTE_V | PTE_U | PTE_RW},
    [VSUR_GURX]       =   {PTE_V | PTE_U | PTE_R,   PTE_V | PTE_U | PTE_RX},
    [VSUR_GUR]        =   {PTE_V | PTE_U | PTE_R,   PTE_V | PTE_U | PTE_R},
    [VSUR_GUX]        =   {PTE_V | PTE_U | PTE_R,   PTE_V | PTE_U | PTE_X},       
    [VSUX_GURWX]      =   {PTE_V | PTE_U | PTE_X,   PTE_V | PTE_U | PTE_RWX},        
    [VSUX_GURW]       =   {PTE_V | PTE_U | PTE_X,   PTE_V | PTE_U | PTE_RW},         
    [VSUX_GURX]       =   {PTE_V | PTE_U | PTE_X,   PTE_V | PTE_U | PTE_RX},
    [VSUX_GUR]        =   {PTE_V | PTE_U | PTE_X,   PTE_V | PTE_U | PTE_R},         
    [VSUX_GUX]        =   {PTE_V | PTE_U | PTE_X,   PTE_V | PTE_U | PTE_X},
    [VSI_GI]          =   {0,                       0}, 
    [VSRWX_GI]        =   {PTE_V | PTE_RWX,         0}, 
    [VSRW_GI]        =    {PTE_V | PTE_RW,          0}, 
    [VSI_GURWX]       =   {0,                       PTE_V | PTE_U | PTE_RWX},  
    [VSI_GUX]         =   {0,                       PTE_V | PTE_U | PTE_X},
    [VSI_GUR]         =   {0,                       PTE_V | PTE_U | PTE_R},
    [VSI_GURW]        =   {0,                       PTE_V | PTE_U | PTE_RW},
    [SCRATCHPAD]      =   {PTE_V | PTE_RWX,         PTE_V | PTE_U | PTE_RWX},           
    [SWITCH1]         =   {PTE_V | PTE_RWX,         PTE_V | PTE_U | PTE_RWX},      
    [SWITCH2]         =   {PTE_V | PTE_RWX,         PTE_V | PTE_U | PTE_RWX},      
    [TOP]             =   {PTE_V | PTE_RWX,         PTE_V | PTE_U | PTE_RWX},     
};      

pte_t hspt[3][PAGE_SIZE/sizeof(pte_t)] __attribute__((aligned(PAGE_SIZE)));

void hspt_init(){

    uintptr_t addr;

    // for(int j = 0; j < 3; j++)
    //     for(int i = 0; i < 512; i++)
    //         hspt[j][i] = 0;

    addr = 0x00000000;
    for(int i = 0; i < 4; i++){
        hspt[0][i] = 
            PTE_V | PTE_AD | PTE_RWX | (addr >> 2);  
        addr +=  SUPERPAGE_SIZE(0);
    }

    hspt[0][4] =
        PTE_V | (((uintptr_t)&hspt[1][0]) >> 2);
    hspt[1][0] = 
        PTE_V | (((uintptr_t)&hspt[2][0]) >> 2);


    addr = TEST_PPAGE_BASE;
    for(int i = 0; i < TEST_PAGE_MAX; i++){
        hspt[2][i] = (addr >> 2) | PTE_AD |
            test_page_perm_table[i].vs;  
        addr += PAGE_SIZE;
    }

    // hspt[0][MEM_BASE/SUPERPAGE_SIZE(0)] = 
    //     PTE_V | (((uintptr_t)&hspt[1][0]) >> 2);

    // addr = MEM_BASE;
    // for(int i = 0; i < 127; i++){
    //     hspt[1][i] = 
    //         PTE_V | PTE_AD | PTE_RWX | (addr >> 2);  
    //     addr +=  SUPERPAGE_SIZE(1);
    // }
    // hspt[1][127] = 
    //     PTE_V | (((uintptr_t)&hspt[2]) >> 2);     

    // addr = 0x80000000 + (127*SUPERPAGE_SIZE(1));
    // for(int i = 0; i < 512; i++){
    //     hspt[2][i] = 
    //         PTE_V | PTE_AD | PTE_RWX | (addr >> 2);  
    //     addr +=  PAGE_SIZE;
    // }

    if(curr_priv == PRIV_HS || curr_priv == PRIV_M){
        uintptr_t satp = (((uintptr_t)hspt) >> 12) | (0x8ULL << 60);
        CSRW(satp, satp);
    } else {
        ERROR("trying to set hs level satp from lower privilege");
    }
}

pte_t vspt[6][PAGE_SIZE/sizeof(pte_t)] __attribute__((aligned(PAGE_SIZE)));

void vspt_init(){

    uintptr_t addr;

    addr = 0x00000000;
    for(int i = 0; i < 4; i++){
        vspt[0][i] = 
            PTE_V | PTE_AD | PTE_RWX | (addr >> 2);  
        addr +=  SUPERPAGE_SIZE(0);
    }

    vspt[0][MEM_BASE/SUPERPAGE_SIZE(0)] = 
        PTE_V | (((uintptr_t)&vspt[1][0]) >> 2);

    addr = MEM_BASE;
    for(int i = 0; i < 512; i++) vspt[1][i] = 0;
    for(int i = 0; i <  MEM_SIZE/SUPERPAGE_SIZE(1)/2; i++){
        vspt[1][i] = 
           PTE_V | PTE_AD | PTE_RWX | (addr >> 2);  
        addr +=  SUPERPAGE_SIZE(1);
    }

    vspt[0][4] =
        PTE_V | (((uintptr_t)&vspt[2][0]) >> 2);

    // vspt[0][5] =
    //     PTE_V | PTE_U | PTE_AD | (((uintptr_t)&vspt[2][0]) >> 2);

    vspt[2][0] = 
        PTE_V | (((uintptr_t)&vspt[3][0]) >> 2);

    addr = TEST_VPAGE_BASE;
    for(int i = 0; i < TEST_PAGE_MAX; i++){
        vspt[3][i] = (addr >> 2) | PTE_AD |
            test_page_perm_table[i].vs;  
        addr +=  PAGE_SIZE;
    }

    vspt[2][1] = 
        PTE_V | (((uintptr_t)&vspt[4][0]) >> 2);

    addr = 4 * SUPERPAGE_SIZE(0) + SUPERPAGE_SIZE(1);
    for(int i = 0; i < 512; i++){
        vspt[4][i] = (addr >> 2) | 
            PTE_V | PTE_AD | PTE_RWX; 
        addr +=  PAGE_SIZE;
    }  

    vspt[0][5] = 
        PTE_V | (((uintptr_t)&vspt[5][0]) >> 2);
    
    addr = 5 * SUPERPAGE_SIZE(0);
    for(int i = 0; i < 512; i++){
        vspt[5][i] = (addr >> 2) |
             PTE_V | PTE_AD | PTE_RWX;  
        addr +=  SUPERPAGE_SIZE(1);
    }  

    uintptr_t satp = (((uintptr_t)vspt) >> 12) | (0x8ULL << 60);
    if(curr_priv == PRIV_VS){
        CSRW(satp, satp);
    } else if(curr_priv == PRIV_HS || curr_priv == PRIV_M){
        CSRW(CSR_VSATP, satp);
    } else {
        ERROR("");
    }
}

pte_t hpt_root[PAGE_SIZE*4/sizeof(pte_t)] __attribute__((aligned(PAGE_SIZE*4)));
pte_t hpt[5][PAGE_SIZE/sizeof(pte_t)] __attribute__((aligned(PAGE_SIZE)));

void hpt_init(){

    for(int i = 0; i < 2048; i++){
        hpt_root[i] = 0;
    }

    uintptr_t addr = 0x0;
    for(int i = 0; i < 4; i++){
        hpt_root[i] = 
            PTE_V | PTE_U | PTE_AD | PTE_RWX | (addr >> 2);  
        addr +=  SUPERPAGE_SIZE(0);
    }

    hpt_root[MEM_BASE/SUPERPAGE_SIZE(0)] =
        PTE_V | (((uintptr_t)&hpt[0][0]) >> 2);

    addr = MEM_BASE;
    for(int i = 0; i < 512; i++) hpt[0][i] = 0;
    for(int i = 0; i < MEM_SIZE/SUPERPAGE_SIZE(1)/2; i++){
        hpt[0][i] = 
            PTE_V | PTE_U | PTE_AD | PTE_RWX | (addr >> 2);  
        addr +=  SUPERPAGE_SIZE(1);
    }    

    hpt_root[4] =
        PTE_V | (((uintptr_t)&hpt[1][0]) >> 2);

    hpt_root[2047] =
        PTE_V | (((uintptr_t)&hpt[1][0]) >> 2);

    hpt[1][0] = 
        PTE_V | (((uintptr_t)&hpt[2][0]) >> 2);

    hpt[1][511] = 
        PTE_V | (((uintptr_t)&hpt[2][0]) >> 2);

    addr = TEST_PPAGE_BASE;
    for(int i = 0; i < TEST_PAGE_MAX; i++){
        hpt[2][i] = (addr >> 2) | PTE_AD |
            test_page_perm_table[i].h;  
        addr +=  PAGE_SIZE;
    }

    hpt[1][1] = 
        PTE_V | (((uintptr_t)&hpt[3][0]) >> 2);
    addr = TEST_PPAGE_BASE;
    for(int i = 0; i < 512; i++){
        hpt[3][i] = (addr >> 2) | 
            PTE_V | PTE_U | PTE_AD | PTE_RWX; 
        addr +=  PAGE_SIZE;
    }  

    hpt_root[5] =
        PTE_V | (((uintptr_t)&hpt[4][0]) >> 2);
    addr = TEST_PPAGE_BASE;
    for(int i = 0; i < 512; i++){
        hpt[4][i] = (addr >> 2) |
             PTE_V | PTE_U | PTE_AD | PTE_RWX;  
        addr +=  SUPERPAGE_SIZE(1);
    }  

    if(curr_priv == PRIV_HS || curr_priv == PRIV_M){
        uintptr_t hsatp = (((uintptr_t)hpt_root) >> 12) | (0x8ULL << 60);
        CSRW(CSR_HGATP, hsatp);
    } else {
        ERROR("trying to set hs hgatp from lower privilege");
    }
}

void hspt_switch(){
    pte_t temp = hspt[2][SWITCH1];
    hspt[2][SWITCH1] = hspt[2][SWITCH2];
    hspt[2][SWITCH2] = temp;
}

void vspt_switch(){
    pte_t temp = vspt[3][SWITCH1];
    vspt[3][SWITCH1] = vspt[3][SWITCH2];
    vspt[3][SWITCH2] = temp;
}

void hpt_switch(){
    pte_t temp = hpt[2][SWITCH1];
    hpt[2][SWITCH1] = hpt[2][SWITCH2];
    hpt[2][SWITCH2] = temp;
}
