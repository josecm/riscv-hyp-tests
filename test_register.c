#include <rvh_test.h>
#include <page_tables.h>

TEST_REGISTER(two_stage_translation);   
TEST_REGISTER(second_stage_only_translation);
TEST_REGISTER(m_and_hs_using_vs_access);
TEST_REGISTER(check_xip_regs);
TEST_REGISTER(interrupt_tests);
TEST_REGISTER(virtual_instruction);
TEST_REGISTER(hfence_test);
