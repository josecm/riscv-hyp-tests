
CROSS_COMPILE ?= riscv64-unknown-elf-
CC:=$(CROSS_COMPILE)gcc
AS:=$(CROSS_COMPILE)as
LD:=$(CROSS_COMPILE)ld
OBJCOPY:=$(CROSS_COMPILE)objcopy
OBJDUMP:=$(CROSS_COMPILE)objdump
READELF:=$(CROSS_COMPILE)readelf

ifneq ($(MAKECMDGOALS), clean)
ifeq ($(PLAT),)
$(error Undefined platform)
endif
endif

log_dir:=log
build_dir:=build/$(PLAT)
plat_dir:=platform/$(PLAT)
ifeq ($(wildcard $(plat_dir)),)
$(error unsupported platform $(PLAT))
else
$(shell mkdir -p $(log_dir))
$(shell mkdir -p $(build_dir))
endif

prev_log_file:=$(build_dir)/prev_log.mk
-include $(prev_log_file)
LOG_LEVEL ?= LOG_INFO
GENERIC_FLAGS += -D LOG_LEVEL=$(LOG_LEVEL)
$(file > $(prev_log_file), PREV_LOG_LEVEL:=$(LOG_LEVEL))
ifneq ($(PREV_LOG_LEVEL), $(LOG_LEVEL))
pre_targets += clean_objs
endif

TARGET := $(build_dir)/rvh_test
c_srcs := main.c page_tables.c rvh_test.c interrupt_tests.c\
	translation_tests.c	test_register.c virtual_instruction.c\
	hfence_tests.c wfi_tests.c tinst_tests.c\
	$(addprefix $(plat_dir)/, $(notdir $(wildcard $(plat_dir)/*.c)))
asm_srcs := boot.S handlers.S  $(wildcard $(plat_dir)/*.S)
ld_file:=linker.ld
inc_dirs := ./inc $(plat_dir)/inc
inc_dirs := $(addprefix -I, $(inc_dirs))

objs:=
objs+=$(patsubst  %.c, $(build_dir)/%.o, $(c_srcs))
objs+=$(patsubst  %.S, $(build_dir)/%.o, $(asm_srcs))
ld_file_final:=$(build_dir)/$(ld_file)

deps:=$(patsubst  %.o, %.d, $(objs)) $(ld_file_final).d
dirs:=$(sort $(dir $(objs) $(deps)))

# For RISC-V GCC versions greater than 10, instructions such as csrwi are
# grouped separately under the zicsr extension and need to be specified
# explicitly by appending '_zicsr' to the -march parameter. Below, the
# variable GCCVERSION only gets defined if the GCC version is > 10.
# See https://github.com/josecm/riscv-hyp-tests/pull/8 for more info
GCCVERSION := $(shell $(CC) -dumpversion | awk -F. '$$1 > 10 { print $$1 }')
ifdef GCCVERSION
	GENERIC_FLAGS += -march=rv64imac_zicsr
else
	GENERIC_FLAGS += -march=rv64imac
endif
GENERIC_FLAGS += -mabi=lp64 -g3 -mcmodel=medany -O3 $(inc_dirs)

ASFLAGS = $(GENERIC_FLAGS)
CFLAGS = $(GENERIC_FLAGS)
LDFLAGS = -ffreestanding -nostartfiles -static $(GENERIC_FLAGS)

all: $(pre_targets) $(TARGET).bin

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(TARGET).elf: $(objs) $(ld_file_final)
	$(CC) $(LDFLAGS) -T$(ld_file_final) $(objs) -o $@
	$(OBJDUMP) -S $@ > $(TARGET).asm
	$(OBJDUMP) -D $@ > $(TARGET).dump
	$(READELF) -a -W $@ > $(@).txt

$(build_dir)/%.o: %.[c,S] $(build_dir)/%.d
	$(CC) $(CFLAGS) -c $< -o $@

$(build_dir)/%.d: %.[c,S,ld] 
	$(CC) $(GENERIC_FLAGS) -MM -MT "$(patsubst %.d, %.o, $@) $@" -MF $@ $<  

$(ld_file_final): $(ld_file)
	$(CC) $(CFLAGS) -E -x assembler-with-cpp $< | grep "^[^#;]" > $@
	
.SECONDEXPANSION:

$(objs) $(deps): | $$(@D)/

$(dirs):
	mkdir -p $@

ifneq ($(MAKECMDGOALS), clean)
-include $(deps)
endif

clean:
	rm -rf $(build_dir)
	rm -rf $(log_dir)

clean_objs:
	rm -rf $(build_dir)/**.o

.PHONY: all clean
