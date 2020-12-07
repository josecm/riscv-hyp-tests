
.section .code_test, "ax"
.global code_test
.option norvc
code_test:
.rept   ((0x2000/4)-1)
    add a0, a0, 1
.endr
    ret

