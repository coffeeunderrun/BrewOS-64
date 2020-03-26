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

# ARCHIVER
AR := $(TARGET)-elf-ar

# COMPILER
CC     := $(TARGET)-elf-gcc
CCFLAG := -std=gnu17 -ffreestanding -nostdlib -masm=intel -mno-red-zone -zmax-page-size=0x1000 -Wall -Wextra
CCKERN := -m64 -mcmodel=kernel -Iinclude/kernel -Iinclude/lib -Lo

ifdef DEBUG
CCKERN += -gdwarf-2 -O0
else
CCKERN += -O3
endif # DEBUG

# LINKER
LD     := $(TARGET)-elf-gcc
LDFLAG :=
LDBOOT := -Tboot/arch/$(TARGET)/stage1.ld -n
LDLOAD := -Tboot/arch/$(TARGET)/stage2.ld -n
LDKERN := -Tkernel/arch/$(TARGET)/kernel.ld

ifndef DEBUG
LDFLAG += -s
endif # DEBUG

BOOT_SRCS := stage1.s bios.s ext2.s
BOOT_OBJS := $(addprefix boot/, $(BOOT_SRCS))
BOOT_OBJS := $(addprefix o/, $(addsuffix .o, $(BOOT_OBJS)))
BOOT_DEPS := $(BOOT_OBJS:.o=.d)

LOAD_SRCS := stage2.s bios.s ext2.s
LOAD_OBJS := $(addprefix loader/, $(LOAD_SRCS))
LOAD_OBJS := $(addprefix o/, $(addsuffix .o, $(LOAD_OBJS)))
LOAD_DEPS := $(LOAD_OBJS:.o=.d)

KERN_SRCS := entry.s arch.s arch.c interrupts.s interrupts.c main.c memory.c
KERN_OBJS := $(addprefix kernel/, $(KERN_SRCS))
KERN_OBJS := $(addprefix o/, $(addsuffix .o, $(KERN_OBJS)))
KERN_DEPS := $(KERN_OBJS:.o=.d)

LIBK_SRCS := mman/kmmap.c
LIBK_SRCS += stdlib/heap.c
LIBK_SRCS += string/memcmp.c string/memcpy.c string/memmove.c string/memset.c
LIBK_SRCS += vector.c
LIBK_OBJS := $(addprefix libk/, $(LIBK_SRCS))
LIBK_OBJS := $(addprefix o/, $(addsuffix .o, $(LIBK_OBJS)))
LIBK_DEPS := $(LIBK_OBJS:.o=.d)

DIRS := o o/boot o/loader o/kernel root
DIRS += o/libk o/libk/stdlib o/libk/string o/libk/mman

all: brewdisk.img

clean: ; @rm -rf *.img $(DIRS)

brewdisk.img: $(DIRS) o/brewboot root/brewload o/libk.a root/brewkern
	@echo "Generating $(@F)..."
	@mke2fs -text2 -droot -F $@ 32M
	@dd if=o/brewboot of=$@ bs=512 count=2 conv=notrunc

o/brewboot: $(BOOT_OBJS)
	@echo "Linking $(@F)..."
	@$(LD) $(CCFLAG) $(CCBOOT) $(LDFLAG) $(LDBOOT) -o$@ $^
	@echo

root/brewload: $(LOAD_OBJS)
	@echo "Linking $(@F)..."
	@$(LD) $(CCFLAG) $(CCLOAD) $(LDFLAG) $(LDLOAD) -o$@ $^
	@echo

root/brewkern: $(KERN_OBJS)
	@echo "Linking $(@F)..."
	@$(LD) $(CCFLAG) $(CCKERN) $(LDFLAG) $(LDKERN) -o$@ $^ -lk
	@echo

o/libk.a: $(LIBK_OBJS)
	@echo "Archiving $(@F)..."
	@$(AR) -rcs $@ $^
	@echo

o/boot/%.s.o: boot/arch/$(TARGET)/%.s
	@echo "Assembling $(@F)..."
	@$(AS) $(ASFLAG) $(ASBOOT) -M $< > $(@:.o=.d)
	@$(AS) $(ASFLAG) $(ASBOOT) -o$@ $<

o/loader/%.s.o: boot/arch/$(TARGET)/%.s
	@echo "Assembling $(@F)..."
	@$(AS) $(ASFLAG) $(ASLOAD) -M $< > $(@:.o=.d)
	@$(AS) $(ASFLAG) $(ASLOAD) -o$@ $<

o/kernel/%.s.o: kernel/arch/$(TARGET)/%.s
	@echo "Assembling $(@F)..."
	@$(AS) $(ASFLAG) $(ASKERN) -M $< > $(@:.o=.d)
	@$(AS) $(ASFLAG) $(ASKERN) -o$@ $<

o/kernel/%.c.o: kernel/arch/$(TARGET)/%.c
	@echo "Compiling $(@F)..."
	@$(CC) $(CCFLAG) $(CCKERN) -MMD -c -o$@ $<

o/kernel/%.c.o: kernel/%.c
	@echo "Compiling $(@F)..."
	@$(CC) $(CCFLAG) $(CCKERN) -MMD -c -o$@ $<

o/libk/%.s.o: lib/%.s
	@echo "Assembling $(@F)..."
	@$(AS) $(ASFLAG) $(ASKERN) -M $< > $(@:.o=.d)
	@$(AS) $(ASFLAG) $(ASKERN) -o$@ $<

o/libk/%.c.o: lib/%.c
	@echo "Compiling $(@F)..."
	@$(CC) $(CCFLAG) $(CCKERN) -MMD -c -o$@ $<

$(DIRS): ; @mkdir -p $@

-include $(BOOT_DEPS) $(LOAD_DEPS) $(KERN_DEPS) $(LIBK_DEPS)
