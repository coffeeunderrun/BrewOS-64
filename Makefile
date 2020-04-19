OS      ?= brewos
BUILD   ?= debug
MACHINE ?= x86_64
TARGET  ?= $(MACHINE)-$(OS)

# SYSROOT
INC_PATH := $(CURDIR)/sysroot/include
LIB_PATH := $(CURDIR)/sysroot/lib

# ASSEMBLER
ifeq ($(MACHINE), x86_64)
AS       := yasm
AS_FLAGS :=
endif

# ARCHIVER
AR       := $(TARGET)-ar
AR_FLAGS :=

# C AND C++ COMPILER FLAGS
C_FLAGS := -Wall -Wextra -Wno-unused-parameter

ifeq ($(MACHINE), x86_64)
C_FLAGS += -masm=intel
endif

# C COMPILER
CC       := $(TARGET)-gcc
CC_FLAGS := -std=gnu17

# C++ COMPILER
CXX       := $(TARGET)-g++
CXX_FLAGS := -std=gnu++17

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
all: boot loader kernel libc

.PHONY: boot loader
boot loader:
	@$(MAKE) --no-print-directory -Cboot $@

.PHONY: kernel
kernel: libk install-libk
	@$(MAKE) --no-print-directory -Ckernel $@

.PHONY: libc libk
libc libk:
	@$(MAKE) --no-print-directory -Clib $@

.PHONY: clean
clean:
	@$(MAKE) --no-print-directory -Cboot $@
	@$(MAKE) --no-print-directory -Ckernel $@
	@$(MAKE) --no-print-directory -Clib $@
	@rm -rf root *.img

.PHONY: install-libc install-libk
install-libc install-libk:
	@$(MAKE) --no-print-directory -Clib $@

hdd.img: all
	@mkdir -p root
	@cp -fP boot/loader kernel/kernel root
	@mke2fs -text2 -droot -F $@ 32M
	@dd if=boot/boot of=$@ bs=512 count=2 conv=notrunc
