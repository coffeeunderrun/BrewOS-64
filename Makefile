TARGET ?= x86_64

# ASSEMBLER
AS     := yasm
ASFLAG := -pnasm
ASBOOT := -felf32
ASLOAD := -felf64 -mamd64
ASKERN := -felf64 -mamd64

ifdef DEBUG
ASKERN += -gdwarf2
endif # DEBUG

# COMPILER
CC     := $(TARGET)-elf-gcc
CCFLAG := -std=gnu17 -ffreestanding -nostdlib -mno-red-zone -zmax-page-size=4096 -Wall
CCBOOT := -m16 -Os -Iboot/include
CCLOAD := $(CCBOOT) -Ikernel/include
CCKERN := -m64 -mcmodel=kernel -Ikernel/include

ifdef DEBUG
CCKERN += -gdwarf-2 -Og
else
CCKERN += -O3
endif # DEBUG

# LINKER
LD     := $(TARGET)-elf-gcc
LDFLAG :=
LDBOOT := -Tboot/$(TARGET)/stage1.ld -n
LDLOAD := -Tboot/$(TARGET)/stage2.ld -n
LDKERN := -Tkernel/$(TARGET)/kernel.ld

ifndef DEBUG
LDFLAG += -s
endif # DEBUG

BOOT_OBJS := o/boot/stage1.s.o o/boot/bios.s.o o/boot/ext2.s.o
BOOT_DEPS := $(BOOT_OBJS:.o=.d)

LOAD_OBJS := o/loader/stage2.s.o o/loader/bios.s.o o/loader/ext2.s.o
LOAD_DEPS := $(LOAD_OBJS:.o=.d)

KERN_OBJS := o/kernel/main.c.o
KERN_DEPS := $(KERN_OBJS:.o=.d)

DIRS := o o/boot o/loader o/kernel root

all: brewdisk.img

clean: ; @rm -rf *.img $(DIRS)

brewdisk.img: $(DIRS) o/brewboot root/brewload root/brewkern
	@echo "Generating $(@F)..."
	@mke2fs -text2 -droot -F $@ 32M
	@dd if=o/brewboot of=$@ bs=512 count=2 conv=notrunc

o/brewboot: $(BOOT_OBJS)
	@echo "Linking $(@F)..."
	@$(LD) $(CCFLAG) $(CCBOOT) $(LDFLAG) $(LDBOOT) -o$@ $^

root/brewload: $(LOAD_OBJS)
	@echo "Linking $(@F)..."
	@$(LD) $(CCFLAG) $(CCLOAD) $(LDFLAG) $(LDLOAD) -o$@ $^

root/brewkern: $(KERN_OBJS)
	@echo "Linking $(@F)..."
	@$(LD) $(CCFLAG) $(CCKERN) $(LDFLAG) $(LDKERN) -o$@ $^

o/boot/%.s.o: boot/$(TARGET)/%.s
	@echo "Assembling $(@F)..."
	@$(AS) $(ASFLAG) $(ASBOOT) -M $< > $(@:.o=.d)
	@$(AS) $(ASFLAG) $(ASBOOT) -o$@ $<

o/loader/%.s.o: boot/$(TARGET)/%.s
	@echo "Assembling $(@F)..."
	@$(AS) $(ASFLAG) $(ASLOAD) -M $< > $(@:.o=.d)
	@$(AS) $(ASFLAG) $(ASLOAD) -o$@ $<

o/kernel/%.s.o: kernel/$(TARGET)/%.s
	@echo "Assembling $(@F)..."
	@$(AS) $(ASFLAG) $(ASKERN) -M $< > $(@:.o=.d)
	@$(AS) $(ASFLAG) $(ASKERN) -o$@ $<

o/kernel/%.c.o: kernel/%.c
	@echo "Compiling $(@F)..."
	@$(CC) $(CCFLAG) $(CCKERN) -MMD -c -o$@ $<

$(DIRS): ; @mkdir -p $@

-include $(BOOT_DEPS) $(LOAD_DEPS) $(KERN_DEPS)
