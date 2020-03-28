ARCH  ?= x86_64
DEBUG ?= 1

IMG_PATH  := ${CURDIR}
INC_PATH  := ${CURDIR}/include
OBJ_PATH  := ${CURDIR}/o
ROOT_PATH := ${CURDIR}/root

AS     := yasm
ASFLAG := -pnasm

AR     := $(ARCH)-elf-ar
ARFLAG :=

CC     := $(ARCH)-elf-gcc
CCFLAG := -std=gnu17 -m64 -masm=intel -Wall -Wextra -I$(INC_PATH) -L$(OBJ_PATH)

LD     := $(ARCH)-elf-gcc
LDFLAG :=

ifneq ($(DEBUG), 1)
LDFLAG += -s
endif

all: libc boot

boot: kernel
	@$(MAKE) --no-print-directory -Cboot $@

kernel: libk
	@$(MAKE) --no-print-directory -Ckernel $@

libc:
	@$(MAKE) --no-print-directory -Clib $@

libk:
	@$(MAKE) --no-print-directory -Clib $@

clean:
	@$(MAKE) --no-print-directory -Cboot $@
	@$(MAKE) --no-print-directory -Ckernel $@
	@$(MAKE) --no-print-directory -Clib $@

.PHONY: all boot kernel libc libk clean

export
