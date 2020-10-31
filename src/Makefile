OS      ?= brewos
BUILD   ?= debug
MACHINE ?= x86_64
TARGET  ?= $(MACHINE)-elf

# ASSEMBLER
ifeq ($(MACHINE), x86_64)
AS       := yasm
AS_FLAGS :=
endif

# C COMPILER FLAGS
CC_FLAGS := -Wall -Wextra -Wno-unused-parameter

ifeq ($(MACHINE), x86_64)
CC_FLAGS += -masm=intel
endif

# C COMPILER
CC       := $(TARGET)-gcc
CC_FLAGS := -std=gnu2x

# LINKER
LD       := $(TARGET)-ld
LD_FLAGS :=

ifeq ($(MACHINE), x86_64)
LD_FLAGS += -zmax-page-size=0x1000
endif

ifneq ($(BUILD), debug)
LD_FLAGS += -s
endif

export

.PHONY: all
all: boot loader kernel

.PHONY: boot loader
boot loader:
	@$(MAKE) --no-print-directory -Cboot $@

.PHONY: kernel
kernel:
	@$(MAKE) --no-print-directory -Ckernel $@

.PHONY: clean
clean:
	@$(MAKE) --no-print-directory -Cboot $@
	@$(MAKE) --no-print-directory -Ckernel $@
	@rm -rf root *.img

brewos.img: all
	@mkdir -p root
	@cp -fP boot/loader kernel/kernel root
	@mke2fs -text2 -droot -F $@ 32M
	@dd if=boot/boot of=$@ bs=512 count=2 conv=notrunc
