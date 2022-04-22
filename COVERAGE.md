# Test Coverages
This document summarizes the coverage of this unit tests suit.

## Two stage translation
+ VS-mode gets right values
+ load guest page fault on unmapped address
+ instruction guest page fault on unmapped 2-stage address
+ invalid pte in both stages

## Second stage only translation
+ VS-mode gets right values
+ VS-mode access to unmapped address
+ access top of guest pa space with high bits == 0
+ access top of guest pa space with high bits != 0

## M and HS using VS access

+ machine sets mprv to access VS-mode space
+ hlvd at HS-mode
+ hlvb and hlvbu at HS-mode
+ hlvh and hlvhu at HS-mode
+ hlvw and hlvwu at HS-mode
+ HS-mode hlvxwu accesses execute-only(x-only) both stage page
+ HS-mode hlvxwu accesses page with all permissions
+ HS-mode hlvxwu on HS-mode-level non-exec page
+ HS-mode hlvxwu on VS-mode-level non-exec page
+ machine mprv is set to VS-mode, access to VU
+ machine mprv is set to VU, access to VU
+ HS-mode hlvd to VU page when spvp = 0
+ HS-mode hlvd to VU page when spvp = 1
+ machine mprv access VS-mode user page when vsstatus.sum set
+ HS-mode hlvd to user page when vsstatus.sum set
+ hs hlvd of x-only both stage page when sstatus.mxr unset
+ hs hlvd of x-only both stage page when sstatus.mxr set
+ hs hlvd of x-only VS-mode page and r-only 2-stage page when sstatus.mxr unset
+ hs hlvd of x-only VS-mode page and r-only 2-stage page when sstatus.mxr set
+ hs hsvb on r-only 2-stage page
+ hs hlvb on r-only 2-stage page
+ hs hsvb on r-only both stage page
+ hs hsvb on invalid 2-stage page

## Interrupts 

+ set sip (vs perspective) by setting all in mip
+ clear sip (vs perspective) by clearing all in mip
+ set sip (vs perspective) by setting all in hvip
+ clear sip (vs perspective) by clearing all in hvip
+ VS-mode software irq (vssi) with no delegation
+ VS-mode software irq (vssi) with delegation to vs

## Virtual instruction

+ VS-mode execute hfence.vvma
+ VS-mode execute hfence.gvma
+ VS-mode execute hlvd
+ VS-mode execute sret when vtsr set
+ VS-mode execute sfence when vtsr set
+ VS-mode acess satp when vtsr set
+ VS-mode execute wfi when vtsr set
+ VS-mode access to time
+ VS-mode access to time when mcounteren.tm and hcounteren.tm set
+ VS-mode access to cycle
+ VS-mode access to cycle when mcounteren.cy set 
+ VS-mode access to cycle when mcounteren.cy and hcounteren.cy set

## hfence test

+ hfences whether invalidate guest tlb entries
+ HS-mode execute sfence whether affect guest tlb entries
+ VS-mode execute sfence whether affect hypervisor tlb entries

## wfi exception

+ U-mode wfi
+ VU-mode wfi
+ machine mode wfi
+ HS-mode wfi
+ HS-mode wfi when mstatus.tw set
+ VS-mode wfi when mstatus.tw set
+ VS-mode wfi when mstatus.tw and hstatus.vtw unset
+ VS-mode wfi when hstatus.vtw set

## TINST tests

+ TINST value when executing a lb which results in a lpf